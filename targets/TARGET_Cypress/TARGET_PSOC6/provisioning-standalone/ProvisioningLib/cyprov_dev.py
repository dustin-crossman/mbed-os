from cyprov_entity import Entity
from cyprov_crypto import crypto
from cyprov_types import types
from cyprov_auth import authorization
from datetime import datetime

class DeviceEntity(Entity) :
    STAGE_VIRGIN= "VIRGIN"
    STAGE_NORMAL= "NORMAL"
    STAGE_SECURE= "SECURE"
    
    def __init__( self , state_name , audit_name ) :
        Entity.__init__( self,state_name,audit_name)
        
    def create_entity( self )  :  
        """
            Creates a virgin device
        """
        self.state["life_cycle_stage"]= self.STAGE_VIRGIN
        
    def install_id( self , cmd ) :
        """
            Installs the die_id, dev_id, cy_pub_key
            This is normally done during the Cypress manufacturing process
            It transitions the device from VIRGIN into NORMAL or SECURE life cycle stage
        """
        if self.state["life_cycle_stage"] != self.STAGE_VIRGIN :
            raise Exception( "Device is not in VIRGIN state" )
            
        self.state["life_cycle_stage"]= self.STAGE_NORMAL
        self.state["cy_pub_key"]= cmd["cy_pub_key"]
        self.state["die_id"]= cmd["die_id"]
        self.state["dev_id"]= cmd["dev_id"]
        self.state["oem_pub_key"]= None
        self.state["dev_pub_key"]= None
        self.state["dev_priv_key"]= None

        # create audit record
        record= {}
        record["type"]= "CY_INSTALL_DEV_ID"
        record["iat"]= datetime.now().isoformat(' ')       # real device has no time, but also no audit file...
        record["cmd"]= cmd
        record["state"]= self.state
        self.append_audit_record(record)
  
    def process_rot_command( self , rot_cmd ) :
        """
            This command establishes the hardware root-of-trust in the device
            It is separate from the provisioning of keys and policies (which may be done more than once)
            This command only installs the OEM public key and creates the device key-pair; 
            the certificate chain for the device is provided with the keys & policies (and can be done more than once too)
        """
        cy_pub_key= self.state["cy_pub_key"]
        die_id= self.state["die_id"]
        life_cycle_stage= self.state["life_cycle_stage"]
        oem_pub_key= self.state["oem_pub_key"]
        dev_pub_key= self.state["dev_pub_key"]
        dev_priv_key= self.state["dev_priv_key"]
        dev_id= self.state["dev_id"]
        
        # get HSM public key and check CY authorization
        rot_cmd_payload= crypto.jwt_payload(rot_cmd)
        cy_auth= rot_cmd_payload["cy_auth"]
        cy_auth_payload= crypto.jwt_payload(cy_auth)
        hsm_pub_key= cy_auth_payload["hsm_pub_key"]
        auth= cy_auth_payload["auth"]
        if not crypto.validate_jwt( cy_auth , cy_pub_key ) :
            raise Exception( "Invalid signature on Cypress HSM authorization" )
        if cy_auth_payload["type"] != types.CY_AUTH_HSM :
            raise Exception( "Invalid type for Cypress HSM authorization" )
        if not authorization.check_authorization(auth,dev_id,die_id) :
            raise Exception( "Cypress did not authorize HSM for this device")
    
        # validate command and get RoT authorization
        prod_id= rot_cmd_payload["prod_id"]
        if not crypto.validate_jwt( rot_cmd , hsm_pub_key ) :
            raise Exception( "Invalid signature on HSM's RoT command" )
        if rot_cmd_payload["type"] != types.HSM_ROT_CMD :
            raise Exception( "Invalid type for HSM's RoT command" )
        
        # validate OEM's RoT authorization
        rot_auth= rot_cmd_payload["rot_auth"]
        rot_auth_payload= crypto.jwt_payload(rot_auth)
        if oem_pub_key==None :
            oem_pub_key= rot_auth_payload["oem_pub_key"]
        if not crypto.validate_jwt( rot_auth , oem_pub_key ) :
            raise Exception( "Invalid signature on OEM's RoT transfer token" )
        if rot_auth_payload["type"] != types.OEM_ROT_AUTH :
            raise Exception( "Invalid type for OEM's RoT transfer token" )
        if rot_auth_payload["prod_id"] != prod_id :
            raise Exception( "Invalid prod_id in OEM's RoT transfer token" )
        if rot_auth_payload["hsm_pub_key"] != hsm_pub_key :
            raise Exception( "OEM's RoT authorization token and Cypress authorization are not for same HSM" )
    
        # Transition to secure if not yet secure
        if life_cycle_stage == self.STAGE_NORMAL :
            life_cycle_state= self.STAGE_SECURE
        if life_cycle_state != self.STAGE_SECURE :
            raise Exception( "Device is not in NORMAL or SECURE life cycle stage" )
        
        # Create device key pair, if not already done
        if dev_priv_key == None :
            dev_priv_key,dev_pub_key= crypto.create_jwk()
        
        # Install RoT if not already done
        if oem_pub_key!=None and oem_pub_key!=rot_auth_payload["oem_pub_key"] :
            oem_pub_key= rot_auth_payload["oem_pub_key"]

        # create the response, based on actual credential (may be different and installed earlier)
        payload= {}
        payload["die_id"]= die_id
        payload["dev_id"]= dev_id
        payload["prod_id"]= prod_id
        payload["dev_pub_key"]= dev_pub_key
        payload["oem_pub_key"]= oem_pub_key
        dev_rsp= crypto.create_jwt( payload , dev_priv_key )
        
        # create audit record
        rot_cmd_readable= crypto.readable_jwt(rot_cmd)
        rot_cmd_readable["payload"]["cy_auth"]= crypto.readable_jwt(rot_cmd_readable["payload"]["cy_auth"])
        rot_cmd_readable["payload"]["rot_auth"]= crypto.readable_jwt(rot_cmd_readable["payload"]["rot_auth"])
        record= {}
        record["type"]= types.HSM_ROT_CMD
        record["iat"]= datetime.now().isoformat(' ')       # real device has not time, but also no audit file...
        record["rot_cmd"]= rot_cmd_readable
        record["state"]= self.state
        self.append_audit_record(record)
        
        self.state["cy_pub_key"]= cy_pub_key
        self.state["die_id"]= die_id
        self.state["life_cycle_stage"]= life_cycle_stage
        self.state["oem_pub_key"]= oem_pub_key
        self.state["dev_pub_key"]= dev_pub_key
        self.state["dev_priv_key"]= dev_priv_key
        self.state["dev_id"]= dev_id
        self.state["prod_id"]= prod_id
        
        return dev_rsp
                
    # Device: process a provision command from HSM
    def process_provision_cmd( self , prov_cmd ) :
        """
            Processes a provisioning command from an HSM:
                prov_req= jwt( payload={ OEM_PROV_REQ , blob } , key=oem_priv_key )
                chain_of_trust= [ x509_certs ]
                hsm_auth= jwt( payload={ CY_HSM_AUTH , hsm_pub_key , auth } , key=cy_priv_key )
                prov_cmd= jwt( payload={ HSM_PROV_CMD , prov_req , chain_of_trust , hsm_auth } , key=hsm_priv_key )
        
            Validates request and installs a keys & policies blob and chain_of_trust for the device
            Returns when successful, throws an error when something went wrong.
        """
        cy_pub_key=     self.state["cy_pub_key"]
        oem_pub_key=    self.state["oem_pub_key"]
        prod_id=        self.state["prod_id"]
        die_id=         self.state["die_id"]
        dev_id=         self.state["dev_id"]
        
        # get hsm public key and check its CY authorization
        prov_cmd_payload= crypto.jwt_payload(prov_cmd)
        cy_auth= prov_cmd_payload["cy_auth"]
        cy_auth_payload= crypto.jwt_payload(cy_auth)
        hsm_pub_key= cy_auth_payload["hsm_pub_key"]
        if not crypto.validate_jwt( cy_auth , cy_pub_key ) :
            raise Exception( "Invalid signature for CY authorization")
        if cy_auth_payload["type"] != types.CY_AUTH_HSM :
            raise Exception( "Invalid type for HSM authorization" )
        if not authorization.check_authorization( cy_auth_payload["auth"] , die_id=die_id, dev_id=dev_id ) :
            raise Exception( "CY authorization not valid for die id={}, dev_id={}".format(die_id,dev_id))

        # validate command
        if not crypto.validate_jwt( prov_cmd , hsm_pub_key ) :
            raise Exception( "Invalid signature for provisioning command" )
        if prov_cmd_payload["type"] != types.HSM_PROV_CMD :
            raise Exception( "Invalid type for provisioning command" )
        prov_req= prov_cmd_payload["prov_req"]
        chain_of_trust= prov_cmd_payload["chain_of_trust"]

        # validate image cert
        image_cert= prov_cmd_payload["image_cert"]
        if not crypto.validate_jwt( image_cert , cy_pub_key ) :
            raise Exception( "Invalid signature of image certificate" )

        # validate OEM's provisioning request
        if not crypto.validate_jwt( prov_req , oem_pub_key ) :
            raise Exception( "Invalid signature for OEM provisioning request" )
        blob= crypto.jwt_payload(prov_req)
        if blob["prod_id"] != prod_id :
            raise Exception( "Provisioning request for wrong product ID {}".format(blob["prod_id"]) )

        # install the blob and chain of trust
        self.state["blob"]= blob
        self.state["chain_of_trust"]= chain_of_trust
       
        # create audit record
        prov_cmd_readable= crypto.readable_jwt(prov_cmd)
        prov_cmd_readable["payload"]["cy_auth"]= crypto.readable_jwt(prov_cmd_readable["payload"]["cy_auth"])
        prov_cmd_readable["payload"]["prov_req"]= crypto.readable_jwt(prov_cmd_readable["payload"]["prov_req"])
        prov_cmd_readable["payload"]["image_cert"]= crypto.readable_jwt(prov_cmd_readable["payload"]["image_cert"])
        record= {}
        record["type"]= types.HSM_PROV_CMD
        record["iat"]= datetime.now().isoformat(' ')       # real device has not time, but also no audit file...
        record["prov_cmd"]= prov_cmd_readable
        record["state"]= self.state
        self.append_audit_record(record)
       