#include <curl/curl.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "newrelic_helper.h"
#include "libnewrelic.h"

int main(void) {
  char* headers;
  newrelic_app_t* app = 0;
  newrelic_txn_t* txn = 0;
  newrelic_app_config_t* config = 0;
  char fullHeader[1000];
  newrelic_external_segment_params_t params = {
      .procedure = "GET",
      .uri = "localhost:8888/test",
  };
  struct curl_slist* list = NULL;
  newrelic_segment_t* segment = 0;
  newrelic_segment_t* root_segment = 0;
  CURLcode res;
  CURL* curl = curl_easy_init();

  if (!curl) {
    return 1;
  }

  example_init();

  char* app_name = get_app_name();
  if (NULL == app_name)
    return -1;

  char* license_key = get_license_key();
  if (NULL == license_key)
    return -1;

  config = newrelic_create_app_config(app_name, license_key);

  config->distributed_tracing.enabled = true;
  customize_config(&config);

  /* Change the transaction tracer threshold to ensure a trace is generated */
  config->transaction_tracer.threshold = NEWRELIC_THRESHOLD_IS_OVER_DURATION;
  config->transaction_tracer.duration_us = 1;

  /* Wait up to 10 seconds for the SDK to connect to the daemon */
  app = newrelic_create_app(config, 10000);
  newrelic_destroy_app_config(&config);

  /* Start a web transaction */
  txn = newrelic_start_web_transaction(app, "ExampleWebTransaction");
  root_segment = newrelic_start_segment(txn, "parent", NULL);

  /* Fake a web request by sleeping for one second. In a more typical
   * instrumentation scenario the start() and stop() calls for the external
   * segment would be before and after code performing an HTTP or SOAP
   * operation, for example. */
  segment = newrelic_start_external_segment(txn, &params);

  headers = newrelic_create_distributed_trace_payload_httpsafe(txn, NULL);
  if (!headers) {
    printf("no headers");
    return 1;
  }
  sprintf(fullHeader, "newrelic: %s", headers);
  list = curl_slist_append(list, fullHeader);
  curl_easy_setopt(curl, CURLOPT_URL, "localhost:8888/test");
  res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

  res = curl_easy_perform(curl);
  res = curl_easy_perform(curl);
  res = curl_easy_perform(curl);
  res = curl_easy_perform(curl);
  res = curl_easy_perform(curl);

  newrelic_end_segment(txn, &segment);
  newrelic_end_segment(txn, &root_segment);

  /* End web transaction */
  newrelic_end_transaction(&txn);

  curl_easy_cleanup(curl);

  newrelic_destroy_app(&app);

  return 0;
}
