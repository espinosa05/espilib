#ifndef __ESPI_JSON_H__
#define __ESPI_JSON_H__

JSON_Status json_load_from_path(struct json *json, const char *path);
JSON_Status json_load_from_fd(struct json *json, int fd);

JSON_Status json_cleanup(struct json *json);

#endif /* __ESPI_JSON_H__ */
