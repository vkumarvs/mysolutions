import json
import logging
import requests

logger = logging.getLogger()
logger.setLevel(logging.INFO)

webhook_url = "https://hooks.slack.com/services/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"
count = 10

def calfactorial(message):
    number = int(message)
    if number == 1:
        return 1
    else:
        number = number * calfactorial(str(number - 1))
    return number

def slackHook(slack_message):
	data = json.dumps(slack_message)
	response = requests.post(webhook_url, json={"text": data}, headers={'Content-Type': 'application/json'})
	if response.status_code != 200:
    		raise ValueError(
        		'Request to slack returned an error %s, the response is:\n%s'
        		% (response.status_code, response.text)
    )

def factorial(event, context):
    #print(event, "\n")
    count = 10
    #print("Triggering calculateHandler1 ", count, "\n")
    count = count + 1
    logger.info("calculateHandler Event: " + str(event))
    message = event['Records'][0]['Sns']['Message']
    logger.info("Message: " + str(message))
    print("Factoria is ", calfactorial(message), "\n")
    slack_message = {'lambda': "executed successfully!"}
    return slackHook(slack_message)

def error(event):
    logger.info("errorHandler Event: " + str(event))
    slack_message = {'lambda': "execution failed!"}
    return slackHook(slack_message)

