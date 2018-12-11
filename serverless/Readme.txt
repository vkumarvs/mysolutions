https://serverless.com/framework/docs/providers/aws/guide/functions/

#Installing a plugin
npm install serverless-python-requirements

#Test your local function copy without deploying that
sls invoke local --function calculate

#When you have made code changes and you want to quickly upload your updated code to AWS Lambda or
just change function configuration. the 
serverless deploy function -f myFunction

#Deployment with stage and region options
serverless deploy --stage production --region eu-central-1


Cheat Sheet
A handy list of commands to use when developing with the Serverless Framework.

#Create A Service:
Creates a new Service

serverless create -p [SERVICE NAME] -t aws-nodejs
#Install A Service
This is a convenience method to install a pre-made Serverless Service locally by downloading the Github repo and unzipping it.

serverless install -u [GITHUB URL OF SERVICE]
#Deploy All
Use this when you have made changes to your Functions, Events or Resources in serverless.yml or you simply want to deploy all changes within your Service at the same time.

serverless deploy -s [STAGE NAME] -r [REGION NAME] -v
#Deploy Function
Use this to quickly overwrite your AWS Lambda code on AWS, allowing you to develop faster.

serverless deploy function -f [FUNCTION NAME] -s [STAGE NAME] -r [REGION NAME]
#Invoke Function
Invokes an AWS Lambda Function on AWS and returns logs.

serverless invoke -f [FUNCTION NAME] -s [STAGE NAME] -r [REGION NAME] -l

#Streaming Logs
Open up a separate tab in your console and stream all logs for a specific Function using this command.

serverless logs -f [FUNCTION NAME] -s [STAGE NAME] -r [REGION NAME]

#Simple IAM role template for lambda function

{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": [
        "cloudformation:*",
        "s3:*",
        "logs:*",
        "iam:*",
        "apigateway:*",
        "lambda:*",
        "ec2:DescribeSecurityGroups",
        "ec2:DescribeSubnets",
        "ec2:DescribeVpcs",
        "events:*"
      ],
      "Resource": [
        "*"
      ]
    }
  ]
}
