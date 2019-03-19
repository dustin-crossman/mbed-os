import click
import json
from jwcrypto import jwk

CUSTOMER_KEY_ID_MIN = 6
CUSTOMER_KEY_ID_MAX = 10

kid_help = 'Key ID to define key slot number in the key storage. Key ID must be in range {}.'

@click.command()
@click.option('-k', '--kid', 'kId',
              type=click.IntRange(CUSTOMER_KEY_ID_MIN, CUSTOMER_KEY_ID_MAX),
              default=CUSTOMER_KEY_ID_MIN, 
              help=kid_help.format(str(list(range(CUSTOMER_KEY_ID_MIN, CUSTOMER_KEY_ID_MAX+1)))))
@click.option('--jwk', 'jwKey',
              type=click.File('w'), 
              default='key.json',
              help='Name of the key in JWK format to create.')
@click.option('--pem-priv', 'pemPriv',
              type=click.File('wb'), 
              default=None,
              help='Name of the private key in PEM format to create. If it is not given PEM file will not be created.')
@click.option('--pem-pub', 'pemPub',
              type=click.File('wb'), 
              default=None,
              help='Name of the public key in PEM format to create. If it is not given PEM file will not be created.')
def main(kId, jwKey, pemPriv, pemPub):
    key = jwk.JWK.generate(kty='EC', crv='P-256', use='sig')
    
    keyJson = json.loads(key.export(private_key=True))
    keyJson['kid'] = str(kId)
    
    keyStr = json.dumps(keyJson, indent=4)
    jwKey.write(keyStr)

    if pemPriv != None:
        pemPriv.write(key.export_to_pem(private_key=True, password=None))

    if pemPub != None:
        pemPub.write(key.export_to_pem(private_key=False, password=None))
    
    print(keyStr)

if __name__ == "__main__":
    main()