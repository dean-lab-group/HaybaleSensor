#!/usr/bin/env python
import httplib
import json
import urllib
import inspect
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import os
import sys

import logging
import urllib2

parent = os.path.splitext(os.path.basename(sys.argv[0]))[0]
if __name__ != parent:
    logger_name = parent + '.' + __name__
else:
    logger_name = __name__
logger = logging.getLogger(logger_name)


class Alerts(object):
    """I should put something informative here."""

    def __init__(self, custom_emails=None, send_alerts=True, api_keys=None, from_email=None, mail_server=None):
        super(Alerts, self).__init__()
        self.send_alerts = send_alerts
        self.custom_emails = custom_emails
        self.api_keys = api_keys
        self.from_email = from_email
        self.mail_server = mail_server
        # Logging for class.
        self.logger = logging.getLogger(logger_name + '.' + self.__class__.__name__)
        self.logger.debug("Starting: " + inspect.stack()[0][3])


    def send_alert(self, msg, subject=None):
        """

        :rtype : True|False
        """
        if not self.send_alerts:
            return False
        if subject is None:
            subject = msg[0:128]
        self.send_mail(subject, self.custom_emails, plain_txt_msg=msg)
        self.send_pushover(subject)
        self.send_slack(msg=msg)
        return True

    def send_mail(self, subject, dest_emails, plain_txt_msg=None, html_txt=None):
        # Create message container - the correct MIME type is multipart/alternative.
        """

        :param subject:
        :param dest_emails:
        :param plain_txt_msg:
        :param html_txt:
        """
        msg = MIMEMultipart('alternative')
        msg['Subject'] = subject
        msg['From'] = self.from_email
        msg['To'] = ', '.join(dest_emails)

        # Record the MIME types of both parts - text/plain and text/html.
        part1 = MIMEText(plain_txt_msg, 'plain')
        msg.attach(part1)

        if html_txt == None:
            pass
        else:
            part2 = MIMEText(html_txt, 'html')
            msg.attach(part2)
        #####

        # Send the message via local SMTP server.
        s = smtplib.SMTP(self.mail_server)
        # if opt.verbose == True:
        # s.set_debuglevel(1)
        ##### END if 

        # sendmail function takes 3 arguments: sender's address, recipient's address
        # and message to send - here it is sent as one string.
        try:
            s.sendmail(self.from_email, dest_emails, msg.as_string())
            s.quit()
        except:
            self.logger.error("Could not send email!")

    def send_pushover(self, msg):
        # Making the message shorter if it's really long, nasty stuff like 
        # the status email.
        """

        :param msg:
        """
        msg = msg[0:160]
        if self.api_keys:
            for dic in self.api_keys:
                apikey = dic['pushover_api_key']
                apptoken = dic['pushover_app_key']
                try:
                    conn = httplib.HTTPSConnection("api.pushover.net:443")
                    conn.request("POST", "/1/messages.json",
                                 urllib.urlencode({
                                     "token": apptoken,
                                     "user": apikey,
                                     "message": msg,
                                 }),
                                 {"Content-type": "application/x-www-form-urlencoded"})
                    conn.getresponse()
                except Exception, e:
                    self.logger.error(e)

    @staticmethod
    def send_slack(msg, hook_url):
        """
        Sends a slack message to the specified channel URL.
        :param msg: Enter a message to send (string).
        """
        url = hook_url
        payload = {
            "text": msg,
            "channel": "#deployment-info",
            "username": "deploy_bot",
            "icon_emoji": ":snake:"
            }
        data = json.dumps(payload)
        try:
            ret = urllib2.urlopen(url, data)
            print ret.read()
        except urllib2.URLError as e:
            print e
        except urllib2.HTTPError as e:
            print ret.read()
            print e



def main():
    logging.basicConfig(level=logging.DEBUG)
    message = """This is a test message from Markus."""
    subject = """Test Email"""
    alert = Alerts(send_alerts=True, custom_emails='first.last@example.com')
    alert.send_alert(message,subject)



if __name__ == '__main__':
    main()
#### END if
