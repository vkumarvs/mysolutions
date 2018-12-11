    @staticmethod
    def get_kinesis_record():
        """
        Generate an item with a random hash key on a large range, and a unique sort key, and  a created date
        """
        item = {'hashKey': randrange(0, 5000000), 'sortKey': str(uuid.uuid4()), 'created': datetime.datetime.utcnow().isoformat()}
        raw_data = json.dumps(item)
        encoded_data = bytes(raw_data)
        kinesis_record = {
            'Data': encoded_data,
            'PartitionKey': str(item['hashKey']),
        }

        return kinesis_record
