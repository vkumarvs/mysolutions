from lib.users import Users
import json
from boto import kinesis
user = "user"
password = "password"
interval = 1500
count = 100
stream_name = "TestStream"
u = Users(user, password, interval, count)
x = u.list()
for line in x.iter_lines():
        kinesis = kinesis.connect_to_region("eu-west-1")
        kinesis.put_record(stream_name, line, "partitionkey")
        if line:
            print (line)
