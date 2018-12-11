import boto3
import json
import logging

logger = logging.getLogger()
logger.setLevel(logging.INFO)
my_session = boto3.session.Session()
region = my_session.region_name
accountId = boto3.client('sts').get_caller_identity().get('Account')
calculateTopic1 = boto3.client('sns')
topicArn= "arn:aws:sns:{}".format(region)+":{}".format(accountId)+":calculate-topic"
print (topicArn)

count = 10

#Here data would be a factorial number to be calculated
def publishSnsTopic(data):
    logger.info("publishSnsTopic: data:" + data)
    response = calculateTopic1.publish(
        TopicArn=topicArn,
        Message=data
    )
    print (response)
    return response

def init(event, context):
    #logger.info("init: event:" + str(event))
    #It should be called from API gateway and my account doesn't support API
    #Gateway so I am hardcoding this number just for demo
    #logger.info("Vipin-init: event:" + event["body"])
    print("Init is being called", count, "\n")
    data = "5"
    if (data.isdigit()):
        logger.info("init: event: digit received:" + data)
    else:
        logger.info("init: event: digit format is wrong:" + data)

    try:
        publishSnsTopic(data)
        print("Going to publish message to SNS topic\n")
    except:
        logger.info("init: event: publishMessage failed to SNS topic:" + data)

    body = {
            "message": "Init function: Event received from S3 bucket upload!",
        "input": data
    }

    response = {
        "statusCode": 200,
        "body": json.dumps(body)
    }

    return response
