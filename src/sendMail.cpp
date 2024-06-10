#include "sendMail.h"

std::vector<std::string> split(const std::string &s, const std::string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

bool sendMail(const Mail& mail, const MailServer& srv) {
    CURL* curl;
    CURLcode res = CURLE_OK;

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = NULL;
        struct curl_slist* recipients = NULL;
        struct curl_slist* slist = NULL;
        curl_mime* mime;
        curl_mime* alt;
        curl_mimepart* part;
        const char** cpp;

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        curl_easy_setopt(curl, CURLOPT_USERNAME, srv.login.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, srv.password.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, srv.server.c_str());

        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, mail.fromEmail.c_str());

        std::string rcpts = "";
        for (auto& rcpt : mail.recipients) {
            rcpts += "\"" + rcpt.name + "\" <" + rcpt.email + ">";
            if (&rcpt != &mail.recipients.back()) {
                rcpts += ", ";
            }
            recipients = curl_slist_append(recipients, rcpt.email.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT_ALLOWFAILS, 1L);

        std::string from = fmt::format("From: \"{}\" <{}>", mail.fromName, mail.fromEmail);
        std::string to = fmt::format("To: {}", rcpts);
        std::string subject = fmt::format("Subject: {}", mail.subject);

        const char* headers_text[] = {
            from.c_str(),
            to.c_str(),
            subject.c_str(),
            NULL
        };

        auto textMessageArr = split(mail.textMessage, "\n");
        std::string textMessage = "";
        for (auto& m : textMessageArr) {
            textMessage += m + "\r\n";
        }
        textMessage += "\r\n";
        const char *inline_text = textMessage.c_str();

        auto htmlMessageArr = split(mail.htmlMessage, "\n");
        std::string htmlMessage = "";
        for (auto& m : htmlMessageArr) {
            htmlMessage += m + "\r\n";
        }
        htmlMessage += "\r\n";
        const char *inline_html = htmlMessage.c_str();

        for (cpp = headers_text; *cpp; cpp++)
            headers = curl_slist_append(headers, *cpp);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        mime = curl_mime_init(curl);
        alt = curl_mime_init(curl);

        part = curl_mime_addpart(alt);
        curl_mime_data(part, inline_html, CURL_ZERO_TERMINATED);
        curl_mime_type(part, "text/html");
        part = curl_mime_addpart(alt);
        curl_mime_data(part, inline_text, CURL_ZERO_TERMINATED);

        part = curl_mime_addpart(mime);
        curl_mime_subparts(part, alt);
        curl_mime_type(part, "multipart/alternative");
        slist = curl_slist_append(NULL, "Content-Disposition: inline");
        curl_mime_headers(part, slist, 1);

        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

        res = curl_easy_perform(curl);

        curl_slist_free_all(recipients);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_mime_free(mime);
        
        if (res != CURLE_OK) {
            fmt::println("Error: {}\n", curl_easy_strerror(res));
            return false;
        }
    }
    return true;
}