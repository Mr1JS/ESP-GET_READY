- Add in the partitions_singleapp.csv --> following line : 
storage,  data, spiffs,  ,        0xF0000,

- create new folder .data <-- add here the content

- create a open file from spiffs with a c file : 
`````c
void init_spiffs(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to mount SPIFFS (%s)", esp_err_to_name(ret));
        return;
    }
}
`````


- then import and use it in ur content:
`````c
static esp_err_t index_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "Serving /index.html");

    FILE *f = fopen("/spiffs/index.html", "r");
    if (!f)
    {
        ESP_LOGE(TAG, "File not found: /spiffs/index.html");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    char buf[512];
    size_t read_bytes;

    httpd_resp_set_type(req, "text/html");

    // Send file in chunks
    while ((read_bytes = fread(buf, 1, sizeof(buf), f)) > 0)
    {
        if (httpd_resp_send_chunk(req, buf, read_bytes) != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to send chunk");
            fclose(f);
            return ESP_FAIL;
        }
    }

    fclose(f);

    // End response
    httpd_resp_send_chunk(req, NULL, 0);

    ESP_LOGI(TAG, "File sent successfully");
    return ESP_OK;
}
`````