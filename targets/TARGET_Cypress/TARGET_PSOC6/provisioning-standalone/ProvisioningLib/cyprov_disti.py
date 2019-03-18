from cyprov_entity import Entity
from cyprov_crypto import crypto
from cyprov_types import types
from datetime import datetime

# Distributor Entity
class DistiEntity(Entity) :
    def __init__( self , state_name , audit_name ) :
        Entity.__init__(self,state_name,audit_name)
                
    def create_entity( self ) :
        """
        Creates the Distributor entity.
        Creates the Distributor main key-pair and returns nothing.
        """
        disti_priv_key,disti_pub_key= crypto.create_jwk()
        self.state["disti_priv_key"]= disti_priv_key
        self.state["disti_pub_key"]= disti_pub_key

    def authorize_hsm( self , auth_req , exp ) :
        """
            Disti creates an authorization JWT for the HSM
            It contains an explicit expiration notice, but not specific privileges
            Expiration is enforced by HSM and visible in audit logs
            Devices have no sense of time so cannot participate
        """
        disti_priv_key= self.state["disti_priv_key"]
        disti_pub_key= self.state["disti_pub_key"]
        
        # validate request
        auth_req_payload= crypto.jwt_payload(auth_req)
        hsm_pub_key= auth_req_payload["hsm_pub_key"]
        if not crypto.validate_jwt( auth_req , hsm_pub_key ) :
            raise Exception( "Invalid signature for HSM's disti_auth request" )
        if auth_req_payload["type"] != types.HSM_REQ_DISTI_AUTH :
            raise Exception( "Invalid type for HSM's dist_auth request" )
        if datetime.fromtimestamp(auth_req_payload["exp"]) < datetime.now() :
            raise Exception( "Request for disti_auth expired" )
        
        # create the authorization
        payload= {}
        payload["type"]= types.DISTI_AUTH_HSM
        payload["hsm_pub_key"]= hsm_pub_key
        payload["disti_pub_key"]= disti_pub_key
        payload["exp"]= int(exp.timestamp())
        disti_auth= crypto.create_jwt( payload , disti_priv_key )
        
        # Create audit record
        record= {}
        record["type"]= "DISTI_AUTH_HSM"
        record["iat"]= datetime.now().isoformat(' ')
        record["disti_auth_req"]= crypto.readable_jwt(auth_req)
        record["dist_auth"]= crypto.readable_jwt(disti_auth)
        self.append_audit_record(record)
        
        return disti_auth
      
    def create_provision_auth( self , prov_req , cnt , exp ) :
        """
            The distributor creates a provisioning command to the HSM by signing the OEM's request
            A distributor command will have an expiration limit and max count that is enforced by the HSM
        """
        disti_priv_key= self.state["disti_priv_key"]
        
        # create the command
        payload= {}
        payload["type"]= types.DISTI_PROV_AUTH
        payload["iat"]= int(datetime.now().timestamp())
        payload["cnt"]= cnt
        payload["exp"]= int(exp.timestamp())
        payload["prov_req"]= prov_req
        prov_auth= crypto.create_jwt( payload,disti_priv_key )

        # create audit record
        prov_req_payload= crypto.jwt_payload(prov_req)
        record= {}
        record["type"]= types.DISTI_PROV_AUTH
        record["iat"]= datetime.now().isoformat(' ') 
        record["prod_id"]= prov_req_payload["prod_id"]
        record["prov_req"]= crypto.readable_jwt(prov_req)
        self.append_audit_record(record)
        
        return prov_auth 
        
      