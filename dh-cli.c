#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>


#define URL  "https://%s.deployhq.com/projects/%s/%s"
#define URL_SIZE    256

static char get_latest_revision(const char *url_getlatest, char *credential)
{
  CURL *curl = NULL;
  CURLcode res;
  struct curl_slist *headers = NULL;

  json_t *root;
  json_error_t error;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url_getlatest);

    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_USERPWD, credential);

    /* url is redirected, so we tell libcurl to follow redirection */ 
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

#ifdef SKIP_PEER_VERIFICATION
    curl_easy_setopt(curl, CURL_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
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

  root = json_loads(res, 0, &error);
  free(res);

  if(!root)
  {
      fprint(stderr, "Error: on line %d: %s\n", error.line, error.text);
      return 1;
  }

  if(!json_is_array(root))
  {
      fprintf(stderr, "Error: root is not an array\n");
      json_decref(root);
      return 1;
  }

  for(i = 0; i < json_array_size(root); i++)
  {
      json_t *name *data;
      const char *name_text;

      data = json_array_get(root, i);
      if(!json_is_object(data))
      {
          fprintf(stderr, "Error: name data %d is not an object\n", (int)(i + 1));
          json_decref(root);
          return 1;
      }

      name = json_object_get(data, "name");
      if(!json_is_string(name))
      {
          fprintf(stderr, "Error: name %d: is not a string\n", (int)(i + 1));
          json_decref(root);
          return 1;
      }

      name_text = json_string_value(name);
      printf("%.8s %.*s\n",
              json_string_value(name),
              newline_offset(name_text),
              name_text);
  }

  json_decref(root);
  return name_text;

error:
  if(name_text)
      free(name_text);
  if(curl)
      curl_easy_cleanup(curl);
  if(headers)
      curl_slist_free_all(headers);
  curl_global_cleanup();
  return NULL;
}

static char get_server_list(const char *url_getservers, char *credential)
{
  CURL *curl = NULL;
  CURLcode res;
  struct curl_slist *headers = NULL;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url_getservers);

    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_USERPWD, credential);

    /* url is redirected, so we tell libcurl to follow redirection */ 
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

#ifdef SKIP_PEER_VERIFICATION
    curl_easy_setopt(curl, CURL_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
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
  return res;
}

int main(int argc, char *argv[]) 
{
    char url[URL_SIZE], url_getlatest[URL_SIZE], end_url_getlatest[URL_SIZE], url_getservers[URL_SIZE];
    struct curl_slist *headers = NULL;
    char *username=getenv("DEPLOYHQ_USER");
    char *token=getenv("DEPLOYHQ_TOKEN");
    char *account=getenv("DEPLOYHQ_ACCOUNT");
    char *text;

    char credential[256];


    if(argc != 3 && argc != 2)
    {
        fprintf(stderr, "usage: %s PROJECT [BRANCH]\n\n", argv[0]);
        return 2;
    }
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    snprintf(url, URL_SIZE, URL, account, argv[1], "/deployments");
    snprintf(url_getservers, URL_SIZE, URL, account, argv[1], "/servers");
    snprintf(credential, sizeof credential, "%s:%s", username, token);

    if(argc == 2)
    {
        snprintf(url_getlatest, URL_SIZE, URL, account, argv[1], "/repository/latest_revision?branch=master");
    }
    else if(argc == 3)
    {
        snprintf(end_url_getlatest, URL_SIZE, "/repository/latest_revision?branch=%s", argv[2]);
        snprintf(url_getlatest, URL_SIZE, URL, account, argv[1], end_url_getlatest);
    }
    get_latest_revision(url_getlatest,credential);

    get_server_list(url_getservers,credential);

    return 0;
}


