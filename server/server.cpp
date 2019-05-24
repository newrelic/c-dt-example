#include <future>
#include <iostream>
#include <set>
#include <string>

#include "httplib.h"

#ifdef USE_NEWRELIC
#include "../libnewrelic.h"
#endif /* USE_NEWRELIC */

using namespace httplib;
using namespace std;

#ifdef USE_NEWRELIC
static newrelic_app_t *app;
int call_count;

bool setup_newrelic() {
  char *license_key = getenv("NEW_RELIC_LICENSE_KEY");

  call_count = 0;

  if (!license_key) {
    cout << "NEW_RELIC_LICENSE_KEY not set." << endl;
    return false;
  }

  char *collector = getenv("NEW_RELIC_HOST");

  if (!collector) {
    cout << "NEW_RELIC_HOST not set." << endl;
    return false;
  }

  if (!newrelic_configure_log("./c_sdk.log", NEWRELIC_LOG_INFO)) {
    cout << "Error configuring logging." << endl;
    return false;
  }

  if (!newrelic_init(NULL, 10)) {
    cout << "Error initializing NR." << endl;
    return false;
  }

  char *app_name = getenv("AGENT_APP_NAME");
  if (!app_name) {
    cout << "AGENT_APP_NAME not set" << endl;
    return false;
  }

  newrelic_app_config_t *config =
      newrelic_create_app_config(app_name, license_key);

  if (!config) {
    cout << "Error creating config." << endl;
    return false;
  }

  /* Change the transaction tracer threshold to ensure a trace is generated */
  config->transaction_tracer.threshold = NEWRELIC_THRESHOLD_IS_OVER_DURATION;
  config->transaction_tracer.duration_us = 0;

  strcpy(config->redirect_collector, collector);

  /* Wait up to 10 seconds for the agent to connect to the daemon */
  app = newrelic_create_app(config, 10000);
  newrelic_destroy_app_config(&config);

  if (!app) {
    cout << "Error initializing app." << endl;
    return false;
  }

  return true;
}
#endif /* USE_NEWRELIC */

#ifdef USE_NEWRELIC
void call_all(const Request &req) {
  newrelic_txn_t* txn = newrelic_start_web_transaction(app, "Example");

  newrelic_external_segment_params_t external_params;
  external_params.procedure = (char *)"GET",
  external_params.uri = (char *)"https://wombats.org/delay/1";

  newrelic_segment_t *ex_segment =
      newrelic_start_external_segment(txn, &external_params);
  newrelic_end_segment(txn, &ex_segment);

  newrelic_end_transaction(&txn);
}
#endif /* USE_NEWRELIC */

int main(void) {
#ifdef USE_NEWRELIC
  if (!setup_newrelic()) {
    return 1;
  }
#endif /* USE_NEWRELIC */

  Server svr;

  svr.Get("/run", [](const Request &req, Response &res) {
#ifdef USE_NEWRELIC
    call_all(req);
#endif /* USE_NEWRELIC */
    res.set_content("Success", "text/plain");
  });

  svr.listen("localhost", 8080);
}
