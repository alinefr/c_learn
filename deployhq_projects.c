#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#define URL_FORMAT  "https://doubleleft.deployhq.com/projects"
#define URL_SIZE    256
 
int main(void)
{
  CURL *curl;
  CURLcode res;
  char url[URL_SIZE];
  char credential[256];
  struct curl_slist *headers = NULL;
  char *username=getenv("DEPLOYHQ_USER");
  char *token=getenv("DEPLOYHQ_TOKEN");

  curl_global_init(CURL_GLOBAL_DEFAULT);

  snprintf(url, URL_SIZE, URL_FORMAT);
  snprintf(credential, sizeof credential, "%s:%s", username, token);

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);

    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_USERPWD, credential);

    /* url is redirected, so we tell libcurl to follow redirection */ 
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

#ifdef SKIP_PEER_VERIFICATION
     /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */ 
    curl_easy_setopt(curl, CURL_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
     /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */ 
    curl_easy_setops(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  return 0;
}
