provider "aws" {
  ...
}

resource "aws_iot_thing" "..." {
  name = ""
  ...
}

resource "aws_kinesis_firehose_delivery_stream" "..." {
  destination = ""
  name = ""
  ...
}

resource "aws_s3_bucket" "..." {
  ...
}
