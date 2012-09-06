#!/usr/bin/env python

from subprocess import call

CERT = "/Users/fred/Desktop/LeClubCert.pem"
KEY = "/Users/fred/Desktop/LeClubKeyNoP.pem"
JSON = '{"aps":{"alert":"test"}}'
TOKENS = ['b7b0ccb0baddab10cca269801c1e6030157a48f864164a6a340259e819d28440']

def send_notifications(cert_path, key_path, json, tokens):
    params = ["./uapn", "-d", cert_path, key_path, json] + tokens
    call(params)

if __name__ == "__main__":
    send_notifications(CERT, KEY, JSON, TOKENS)
