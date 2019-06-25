#include <curl/curl.h>
#include <stdlib.h>

#include "newrelic_helper.h"
#include "libnewrelic.h"

static newrelic_app_t* app;

bool setup_newrelic() {
  newrelic_app_config_t* config = 0;

  /* Configure the logs and initialize the C SDK */
  example_init();

  char* app_name = get_app_name();
  if (NULL == app_name)
    return false;

  char* license_key = get_license_key();
  if (NULL == license_key)
    return false;

  config = newrelic_create_app_config(app_name, license_key);

  /* Turn on distributed tracing */
  config->distributed_tracing.enabled = true;
  customize_config(&config);

  /* Wait up to 10 seconds for the SDK to connect to the daemon */
  app = newrelic_create_app(config, 10000);
  newrelic_destroy_app_config(&config);

  return true;
}

bool send_http_request() {
  char* headers;
  newrelic_txn_t* txn = 0;
  char fullHeader[1000];

  newrelic_external_segment_params_t params = {
          .procedure = "GET",
          .uri = "localhost:8080/test",
  };
  struct curl_slist* list = NULL;
  newrelic_segment_t* segment = 0;
  newrelic_segment_t* root_segment = 0;
  CURLcode res;
  CURL* curl = curl_easy_init();

  if (!curl) {
    printf("Curl init failed, exiting");
    return false;
  }

  /* Start a web transaction */
  txn = newrelic_start_web_transaction(app, "ExampleWebTransaction");
  root_segment = newrelic_start_segment(txn, "parent", NULL);

  /* Start an external segment */
  segment = newrelic_start_external_segment(txn, &params);

  /* Create a distributed trace payload */
  headers = newrelic_create_distributed_trace_payload_httpsafe(txn, NULL);
  if (!headers) {
    printf("newrelic_create_distributed_trace_payload_httpsafe failed, exiting");
    return false;
  }
  /* New Relic products look for distributed trace payload with the header name 'newrelic'. */
  sprintf(fullHeader, "newrelic: %s", headers);
  list = curl_slist_append(list, fullHeader);
  curl_easy_setopt(curl, CURLOPT_URL, "localhost:8080/test");
  res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

  res = curl_easy_perform(curl);

  if (CURLE_OK != res) {
    printf("curl_easy_perform failed with a response code of: %s\n", curl_easy_strerror(res));
    return false;
  }

  newrelic_end_segment(txn, &segment);
  newrelic_end_segment(txn, &root_segment);

  /* End web transaction */
  newrelic_end_transaction(&txn);

  curl_easy_cleanup(curl);

  free(headers);
  return true;
}

int main(void) {
  if (!setup_newrelic()) {
    return -1;
  }

  if (!send_http_request()) {
    return -1;
  }

  newrelic_destroy_app(&app);

  return 0;
}
