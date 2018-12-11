'''
Slack chatboat using lambda blueprint
1. Configure the bot with a slack webhook to post the message to the slack channela
2. Encrypt the URL of the  webhook and base 64 encode it
3. Create a IAM role with permission to decrypt the key

Configure incoming webhook:
1.Sign into your slack account
2.In the navigation menu click the Apps
3.In the Browse Apps type Incoming Webhooks
4.Click Install
5.Click Add Configuration
6.Click the post to channel and select #general
7.Click Add Incoming WebHooks  integration
8.  incoming hooks URL:
https://hooks.slack.com/services/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
9. Save the settings

Create and subscribe to an SNS topic: (Pub sub based messaging at any scale)
1.Under Services:  Click Simple Notification Service
2. Get started of create Topic
    Topic name: slacknews
    Display Name: slacknews
    click Create Topic
3.Create Subscription the configure
 Protocol: Email
 Endpoint: Enter your email address
Create subscription
4.Check your email for slacknews confirmation
5.Click the Confirm Subscription link in the slacknews email to confirm the subscription

Create a Lambda Function:
1. Under Services click Lambda
2. Create Function
3.Click Blueprints
4. On the Blueprint search box enter "cloudwatch-alarm-to-slack-python"
5. Enter
6. Configure
  Name: Slackfunction
  Role: Choose an existing Role
  Existing Role: Lambda-role
7. In the SNS trigger section click Enable trigger
8. In the evironment variable section remove the predefined variables
9. Create Function, In the function code delete all the code and use the code written here

10. In the base setting configuration
    Memory: 1024MB
    Timeout: 5 min
    click save

11. In my lambda SlackFunction click Test the configure
    Event template: Search for and select "Amazon SNS Topic Notification"
    Event Name: "SlackEvent"
12. Copy and paste the event alarm template code and create and test. Should be successful
'''

from __future__ import print_function

import boto3
import json
import logging

from base64 import b64decode
from urllib2 import Request, urlopen, URLError, HTTPError


SLACK_CHANNEL = '#general'

HOOK_URL = "https://hooks.slack.com/services/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"

logger = logging.getLogger()
logger.setLevel(logging.INFO)


def lambda_handler(event, context):
    logger.info("Event: " + str(event))
    message = event['Records'][0]['Sns']['Message']
    logger.info("Message: " + str(message))

    alarm_name = message['AlarmName']
    #old_state = message['OldStateValue']
    new_state = message['NewStateValue']
    reason = message['NewStateReason']

    slack_message = {
        'channel': SLACK_CHANNEL,
        'text': "%s state is now %s: %s" % (alarm_name, new_state, reason)
    }

    req = Request(HOOK_URL, json.dumps(slack_message))
    try:
        response = urlopen(req)
        response.read()
        logger.info("Message posted to %s", slack_message['channel'])
    except HTTPError as e:
        logger.error("Request failed: %d %s", e.code, e.reason)
    except URLError as e:
        logger.error("Server connection failed: %s", e.reason)

'''
Event template code:
{
  "Records": [
    {
      "EventVersion": "1.0",
      "EventSubscriptionArn": "arn:aws:sns:EXAMPLE",
      "EventSource": "aws:sns",
      "Sns": {
        "SignatureVersion": "1",
        "Timestamp": "1970-01-01T00:00:00.000Z",
        "Signature": "EXAMPLE",
        "SigningCertUrl": "EXAMPLE",
        "MessageId": "95df01b4-ee98-5cb9-9903-4c221d41eb5e",
        "Message": {
          "AlarmName": "SlackAlarm",
          "NewStateValue": "OK",
          "NewStateReason":"Threshold Crossed: 1 datapoint (0.0) was not greater than or equal to the threshold (1.0)."
        },
        "MessageAttributes": {
          "Test": {
            "Type": "String",
            "Value": "TestString"
          },
          "TestBinary": {
            "Type": "Binary",
            "Value": "TestBinary"
          }
        },
        "Type": "Notification",
        "UnsubscribeUrl": "EXAMPLE",
        "TopicArn": "arn:aws:sns:EXAMPLE",
        "Subject": "TestInvoke"
      }
    }
  ]
}
'''


