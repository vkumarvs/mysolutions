#This tutorial is for study purpose and it takes references from various resources
Reference doc link: https://medium.com/retailmenot-engineering/building-a-high-throughput-data-pipeline-with-kinesis-lambda-and-dynamodb-7d78e992a02d


Amazon Kinesis:
Shard:
A shard is the base throughput unit of a stream.
One shard provides a capacity of 1MB/sec data input and 2MB/sec data output. One shard can support up to 1000 PUT records per second.
A shard is described by:
ShardId
EndingHashKey + StartingHashKey
SequenceNumberRange

Data Record:
A record is the unit of data stored in a stream.
Inside a record you will find:
A sequence number
A partition key
Data blob (1 megabyte (MB) maximum)

Partition Key
Partition key is used to segregate and route data records to different shards of a stream.
You can create more than one shard in order to send all the data with a specific partition key to a specific shard.

Sequence Number
A sequence number is a unique identifier for each data record.
When a developer calls PutRecord from the data producer, a sequence number is assigned by Amazon Kinesis Streams to the data sequence.

Amazon Kinesis Firehose Concepts
Firehose is also an Amazon managed service intended to delivering real-time streaming data to destinations :
Amazon S3, Amazon Redshift and Amazon Elasticsearch Service
The advantage of using Firehose in our case is that we can deliver the streaming data to a data store without
writing any code. A simple configuration from that AWS console is sufficient.
