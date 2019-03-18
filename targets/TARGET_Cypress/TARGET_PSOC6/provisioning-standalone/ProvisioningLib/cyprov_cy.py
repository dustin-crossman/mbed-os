from cyprov_entity import Entity
from cyprov_crypto import crypto
from cyprov_types import types
from datetime import datetime
from datetime import timedelta
from intelhex import IntelHex
import sys
import hashlib

# Cypress Entity
class CypressEntity(Entity) :
    def __init__( self , state_name , audit_name ) :
        Entity.__init__(self,state_name,audit_name)
        
    def create_entity( self )  :  
        """
        Creates the Cypress entity.
        Creates the Cypress main key-pair and returns it as JWK.
        """
        cy_priv_key,cy_pub_key= crypto.create_jwk()
        self.state["cy_priv_key"]= cy_priv_key
        self.state["cy_pub_key"]= cy_pub_key
        
    def create_bootloader_jwt( self, exp, bootloader_id, bootloader_file, toc3_addr, jwt_name )  :  
        """
        Creates items signed with Cypress private key: cy_bootloader.
        """
        cy_pub_key= self.state["cy_pub_key"]
        cy_priv_key= self.state["cy_priv_key"]
        
        IMAGE_ADDR_OFFST = 16
        IMAGE_SIZE_OFFST = 20

        ih = IntelHex(bootloader_file)
        ih.padding = 0x00
        segments = ih.segments()
        toc3_size_arr = ih.tobinarray(start = segments[1][0], size = 4)
        toc3_size = toc3_size_arr[0] + toc3_size_arr[1] * 256 + toc3_size_arr[2] * 256 * 256 + toc3_size_arr[3] * 256 * 256 * 256
        image_addr_arr = ih.tobinarray(start = (segments[1][0] + IMAGE_ADDR_OFFST), size = 4)
        image_addr = image_addr_arr[0] + image_addr_arr[1] * 256 + image_addr_arr[2] * 256 * 256 + image_addr_arr[3] * 256 * 256 * 256
        image_size_arr = ih.tobinarray(start = (segments[1][0] + IMAGE_SIZE_OFFST), size = 4)
        image_size = image_size_arr[0] + image_size_arr[1] * 256 + image_size_arr[2] * 256 * 256 + image_size_arr[3] * 256 * 256 * 256
        
        sha256 = hashlib.sha256()
        sha256.update(ih.tobinarray(start = image_addr, size = image_size))
        sha256.update(ih.tobinarray(start = toc3_addr, size = toc3_size))

        bootloader_hash = sha256.digest()
        
        # create the authorization
        payload= {}
        payload["image_id"]= bootloader_id
        payload["image_hash"]= list(bootloader_hash)
        payload["image_file"]= bootloader_file
        payload["image_address"]= image_addr
        payload["policy_template"]= ""
        payload["cy_pub_key"]= cy_pub_key
        payload["iat"]= int(datetime.now().timestamp())
        payload["exp"]= int(exp.timestamp())
        cy_bootloader_jwt= crypto.create_jwt( payload , cy_priv_key )

        print( "  (saving as jwt)" )
        crypto.dump_jwt( cy_bootloader_jwt, jwt_name )

        # create audit record
        record= {}
        record["image_id"]= bootloader_id
        record["image_hash"]= list(bootloader_hash)
        record["image_file"]= bootloader_file
        record["image_address"]= image_addr
        record["policy_template"]= ""
        record["cy_pub_key"]= cy_pub_key
        record["iat"]= datetime.now().isoformat(' ')
        self.append_audit_record(record)
        
    def create_entrance_exam_signed_jwt( self, flashboot_file )  :  
        """
        Creates items signed with Cypress private key: entrance_exam_signed.jwt.
        """
        cy_pub_key= self.state["cy_pub_key"]
        cy_priv_key= self.state["cy_priv_key"]
        
    def install_device_id( self , die_id , dev_id ) :
        """
            This step emulates the Cypress manufacturing process where the Cypress public key and (dev_id/die_id) are installed
        """
        # create the command (not signed)
        cmd= {}
        cmd["type"]= types.CY_DEV_ID
        cmd["die_id"]= die_id
        cmd["dev_id"]= dev_id
        cmd["cy_pub_key"]= self.state["cy_pub_key"]
        
        # create audit record
        record= {}
        record["type"]= types.CY_DEV_ID
        record["iat"]= datetime.now().isoformat(' ')
        record["cmd"]= cmd
        self.append_audit_record(record)
        
        return cmd
        
    def authorize_hsm( self , auth_req , disti_pub_key , exp , auth ) :
        """
            Authorize HSM by CY
            First validate the HSM is authorized by the specified distributor
            Then create an authorization JWT with specified expiration limit and privileges
        """
        cy_pub_key= self.state["cy_pub_key"]
        cy_priv_key= self.state["cy_priv_key"]
        
        # validate the HSM request
        auth_req_payload= crypto.jwt_payload(auth_req)
        hsm_pub_key= auth_req_payload["hsm_pub_key"]
        if not crypto.validate_jwt( auth_req , hsm_pub_key ) :
            raise Exception( "Invalid signature for HSM authorization request" )
        if auth_req_payload["type"] != types.HSM_REQ_CY_AUTH :
            raise Exception( "Invalid type for HSM authorization request" )
        if datetime.fromtimestamp(auth_req_payload["exp"]) < datetime.now() :
            raise Exception( "Authorization request expired" )
        if "cy_pub_key" in auth_req_payload :
            if auth_req_payload["cy_pub_key"] != cy_pub_key :
                raise Exception( "Authorization request contains an invalid Cypress public key" )
        
        # Validate the distributor authorization
        disti_auth= auth_req_payload["disti_auth"]
        disti_auth_payload= crypto.jwt_payload(disti_auth)
        if disti_pub_key==None :
            print( "No distributor public key given, using key from distributor authorization" )
            disti_pub_key= disti_auth_payload["disti_pub_key"]
        if not crypto.validate_jwt( disti_auth , disti_pub_key ) :
            raise Exception( "Distributor authorization is invalid/not from the expected distributor" )
        if disti_auth_payload["type"] != types.DISTI_AUTH_HSM :
            raise Exception( "Invalid type for distributor authorization")
        if disti_auth_payload["hsm_pub_key"] != hsm_pub_key :
            raise Exception( "Distributor authorization is not for this HSM" )
        if datetime.fromtimestamp(disti_auth_payload["exp"]) < datetime.now() :
            raise Exception( "Distributor authorization expired" )
            
        # create the authorization
        payload= {}
        payload["type"]= types.CY_AUTH_HSM
        payload["hsm_pub_key"]= hsm_pub_key
        payload["cy_pub_key"]= cy_pub_key
        payload["iat"]= int(datetime.now().timestamp())
        payload["exp"]= int(exp.timestamp())
        payload["auth"]= auth
        cy_auth= crypto.create_jwt( payload , cy_priv_key )
        
        # create audit record
        auth_req_readable= crypto.readable_jwt(auth_req)
        auth_req_readable["payload"]["disti_auth"]= crypto.readable_jwt(auth_req_readable["payload"]["disti_auth"])
        record= {}
        record["type"]= types.CY_AUTH_HSM
        record["iat"]= datetime.now().isoformat(' ')
        record["auth_req"]= auth_req_readable
        record["cy_auth"]= crypto.readable_jwt(cy_auth)
        self.append_audit_record(record)
        
        return cy_auth
        
      