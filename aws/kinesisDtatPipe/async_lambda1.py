import base64
import json
import boto3
import datetime


def lambda_handler(event, context):
    """
    Receive a batch of events from Kinesis and insert as-is into our DynamoDB table if invoked asynchronously,
    otherwise perform an asynchronous invocation of this Lambda and immediately return
    """
    if not event.get('async'):
        invoke_self_async(event, context)
        return

    print('Received request')
    item = None
    dynamo_db = boto3.resource('dynamodb')
    table = dynamo_db.Table('demo_workflow_data')
    decoded_record_data = [base64.b64decode(record['kinesis']['data']) for record in event['Records']]
    deserialized_data = [json.loads(decoded_record) for decoded_record in decoded_record_data]

    with table.batch_writer() as batch_writer:
        for item in deserialized_data:
            # Add a processed time so we have a rough idea how far behind we are
            item['processed'] = datetime.datetime.utcnow().isoformat()
            batch_writer.put_item(Item=item)

    # Print the last item to make it easy to see how we're doing
    print(json.dumps(item))
    print('Number of records: {}'.format(str(len(deserialized_data))))


def invoke_self_async(event, context):
    """
    Have the Lambda invoke itself asynchronously, passing the same event it received originally,
    and tagging the event as 'async' so it's actually processed
    """
    event['async'] = True
    called_function = context.invoked_function_arn
    boto3.client('lambda').invoke(
        FunctionName=called_function,
        InvocationType='Event',
        Payload=bytes(json.dumps(event))
    )
