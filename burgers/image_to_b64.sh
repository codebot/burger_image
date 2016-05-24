#!/bin/bash
base64 <$1 | tr -d '\n' > $1.b64
