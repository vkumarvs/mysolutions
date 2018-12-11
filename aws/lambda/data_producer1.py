import boto3


class KinesisLoader:
    def __init__(self, batch_size=500, maximum_records=None):
        """
        The default batch_size here is to match the maximum allowed by Kinesis in a PutRecords request
        """
        self.batch_size = min(batch_size, 500)
        self.maximum_records = maximum_records
        self.kinesis_client = boto3.session.Session().client('kinesis')

    def generate_and_submit(self):
        counter = 0
        # Simple cutoff here - guaranteed to not send in more than maximum_records, with single batch granularity
        while counter < self.maximum_records and counter < self.maximum_records - self.batch_size:
            records_batch = [self.get_kinesis_record() for _ in xrange(0, self.batch_size)]
            request = {
                'Records': records_batch,
                'StreamName': 'workflow_data_stream'
            }

            self.kinesis_client.put_records(**request)
            print('Batch inserted. Total records: {}'.format(str(counter)))

        return
