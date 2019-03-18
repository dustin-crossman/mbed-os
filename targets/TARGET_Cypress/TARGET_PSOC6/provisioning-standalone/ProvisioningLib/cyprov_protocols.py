from cyprov_cy import CypressEntity
from cyprov_disti import DistiEntity
from cyprov_oem import OemEntity
from cyprov_dev import DeviceEntity
from cyprov_hsm import HsmEntity
from cyprov_customer import CustomerEntity
from cyprov_crypto import crypto
import json

class cyprov :
        
    # protocols:
    # 1. Create entities (CY,DISTI,OEM)
    def protocol_create_entities(exp, image_id, image_file, image_toc3_addr, image_jwt_file, flashboot_image_file) :
        print( "\n\n###### protocol_create_entities()" )
        
        cy= CypressEntity( "cy_state_internal.json" , "cy_log.json" )
        oem= OemEntity( "oem_state.json" , "oem_log.json" )
        disti= DistiEntity( "disti_state.json" , "disti_log.json" )
    
        if cy.state_loaded != True :
            print( "Step 1. Create a new Cypress entity" )
            cy.create_entity()
        
    
        if oem.state_loaded != True :
            print( "Step 2. Create a new OEM entity" )
            oem.create_entity()
        
        
        if cy.state_loaded != True :
            print( "Step 3. Create a new distributor entity" )
            disti.create_entity()
        
        print( "Protocol Complete; saving state and committing logs" )
        cy.save_state()
        oem.save_state()
        disti.save_state()
        
        cy.create_bootloader_jwt( exp, image_id, image_file, image_toc3_addr, image_jwt_file )
    
    # 2. Disti creates and authorizes an HSM
    def protocol_disti_creates_hsm( hsm_name,exp ) :
        print( "\n\n###### protocol_disti_creates_hsm( hsm_name={}, exp={})".format(hsm_name,exp) )
        
        hsm= HsmEntity( hsm_name+"_state.json" , hsm_name+"_log.json" )
        disti= DistiEntity( "disti_state.json" , "disti_log.json" )
    
        print(hsm.state_name, hsm.state_loaded)
    
        if hsm.state_loaded != True :
            print( "Step 1. Create a new HSM entity" )
            hsm.create_entity()
    
        print( "Step 2. HSM creates a request for authorization with its public key in it" )
        disti_auth_req= hsm.request_disti_authorization() 
        print( "  (saving as disti_auth_req.jwt)" )
        crypto.dump_jwt( disti_auth_req,"disti_auth_req.jwt" )
    
        print( "Step 3. Distributor authorizes the HSM for provisioning" )
        disti_auth= disti.authorize_hsm(disti_auth_req,exp)
        print( "  (saving as disti_auth.jwt)" )
        crypto.dump_jwt( disti_auth,"disti_auth.jwt" )
    
        print( "Step 4. The HSM installs DISTI authorization" )
        hsm.install_disti_auth(disti_auth )

        print( "Protocol Complete; saving state and committing logs" )
        hsm.save_state()
        disti.save_state()

    # 3. Cypress authorizes HSM
    def protocol_cypress_authorizes_hsm( hsm_name,disti_pub_key,exp,auth,cy_pub_key=None ) :
        print( "\n\n###### protocol_cypress_authorizes_hsm( hsm_name={}, disti_pub_key=..., exp={}, cy_pub_key=... )".format(hsm_name,exp) )
        
        hsm= HsmEntity( hsm_name+"_state.json" , hsm_name+"_log.json" )
        cy= CypressEntity( "cy_state_internal.json" , "cy_log.json" )
    
        print( "Step 1. HSM creates an authorization request with its public key in it" )
        cy_auth_req= hsm.request_cy_authorization(cy_pub_key)
        print( "  (saving as cy_auth_req.jwt)" )
        crypto.dump_jwt( cy_auth_req,"cy_auth_req.jwt" )
    
        print( "Step 2. Cypress authorizats the HSM" )
        cy_auth= cy.authorize_hsm(cy_auth_req,disti_pub_key,exp,auth)
        print( "  (saving as cy_auth.jwt)" )
        crypto.dump_jwt( cy_auth,"cy_auth.jwt" )
    
        print( "Step 3. The HSM installs CY authorization" )
        hsm.install_cy_auth(cy_auth) 
    
        print( "Protocol Complete; saving state and committing logs" )
        hsm.save_state()
        cy.save_state()
    
    # 4. OEM obtains a signing key and authorizes it
    def protocol_oem_signing_key( hsm_name,prod_id ) :
        print( "\n\n###### protocol_oem_signing_key( hsm_name={}, prod_id={} )".format(hsm_name,prod_id) )
        
        hsm= HsmEntity( hsm_name+"_state.json" , hsm_name+"_log.json" )
        oem= OemEntity( "oem_state.json" , "oem_log.json" )

        print( "Step 1. HSM creates a 'package' with the signing key" )
        signing_pkg= hsm.create_signing_key(prod_id)
        print( "  (saving as signing_pkg.jwt)" )
        crypto.dump_jwt( signing_pkg,"signing_pkg.jwt" )
    
        print( "Step 2. OEM authorizes the HSM and sets up a chain of trust" )
        rot_auth_pkg= oem.create_rot_authorization(signing_pkg)
        print( "  (saving as rot_auth_pkg.jwt)" )
        crypto.dump_jwt( rot_auth_pkg,"rot_auth_pkg.jwt" )
    
        print( "Step 3. HSM accepts and installs that package to prepare for device provisioning" )       
        hsm.install_rot_authorization(rot_auth_pkg)     

        print( "Protocol Complete; saving state and committing logs" )
        hsm.save_state()
        oem.save_state()

    # 5. OEM requests a provioning operation, DISTI commands it and HSM installs it
    def protocol_oem_request( hsm_name,blob,cnt,exp ) :
        print( "\n\n###### protocol_oem_request( hsm_name={}, blob=... , cnt={}, exp={} )".format(hsm_name,cnt,exp) )
        
        hsm= HsmEntity( hsm_name+"_state.json" , hsm_name+"_log.json" )
        oem= OemEntity( "oem_state.json" , "oem_log.json" )
        disti= DistiEntity( "disti_state.json" , "disti_log.json" )
        
        customer_key_n = 5
        customer_key_id = 6
        customer_state = "customer_state_{}.json"
        customer_log = "customer_log_{}.json"
        customer = []
        for i in range(customer_key_n):
            customer.append(CustomerEntity(customer_state.format(customer_key_id+i), customer_log.format(customer_key_id+i)))        
            if customer[i].state_loaded != True:
                customer[i].create_entity(customer_key_id+i)
            customer[i].save_state()

        blob["custom_pub_key"] = [key.get_pub_key() for key in customer]
    
        print( "Step 1. The OEM creates a request for device provisioning" )
        prov_req= oem.create_provision_request(blob)    
        print( "  (saving as prov_req.jwt)" )
        crypto.dump_jwt( prov_req,"prov_req.jwt" )
    
        print( "Step 2. The distributor authorizes the HSM to provision <cnt> devices before <exp>" )
        prov_auth= disti.create_provision_auth(prov_req,cnt,exp)
        print( "  (saving as prov_auth.jwt)" )
        crypto.dump_jwt( prov_auth,"prov_auth.jwt" )
    
        print( "Step 3. The HSM accepts the command in preparaton for provisioning" )
        hsm.accept_provision_authorization(prov_auth)

        print( "Protocol Complete; saving state and committing logs" )
        hsm.save_state()
        oem.save_state()
        disti.save_state()

    # 6. CY manufactures a device 
    def protocol_manufacture_device(dev_name,die_id,dev_id) :
        print( "\n\n###### protocol_manufacture_device( dev_name={}, die_id={}, dev_id={} )".format(dev_name,die_id,dev_id) )
        
        cy= CypressEntity( "cy_state_internal.json" , "cy_log.json" )
        dev= DeviceEntity( dev_name+"_state.json" , dev_name+"_log.json" )
    
        print( "Step 1. The device is manufactured in VIRGIN state" )
        dev.create_entity()
    
        print( "Step 2. Cypress creates a command that contains die_id, dev_id and cy_pub_key" )
        id_cmd= cy.install_device_id(die_id,dev_id)
        print( "  (saving as id_cmd.json)" )
        with open("id_cmd.json","w") as f :
            json.dump(id_cmd,f)
    
        print( "Step 3. The device receives the IDs as part of assembly & test" )
        dev.install_id(id_cmd)
   
        print( "Protocol Complete; saving state and committing logs" )
        cy.save_state()
        dev.save_state()
    
    # 7. HSM provisions a device 
    def protocol_provision_device(hsm_name,dev_name,prod_id,image_jwt_file) :
        print( "\n\n###### protocol_provision_device( hsm_name={}, dev_name={}, prod_id={} )".format(hsm_name,dev_name,prod_id) )
        
        hsm= HsmEntity( hsm_name+"_state.json" , hsm_name+"_log.json" )
        dev= DeviceEntity( dev_name+"_state.json" , dev_name+"_log.json" )
    
        print( "Step 1. HSM creates a root-of-trust transfer command" )
        rot_cmd= hsm.create_rot_command(prod_id)
        print( "  (saving as rot_cmd.jwt)" )
        crypto.dump_jwt( rot_cmd,"rot_cmd.jwt" )
    
        print( "Step 2. Device installs the new root-of-trust and returns its device ID and public key" )
        dev_rsp= dev.process_rot_command(rot_cmd)   
        print( "  (saving as dev_rsp.jwt)" )
        crypto.dump_jwt( dev_rsp,"dev_rsp.jwt" )
    
        print( "Step 3. HSM creates the provisioning command (this can be repeated depending on policies defined in blob)" )
        prov_cmd= hsm.create_provision_cmd(prod_id,dev_rsp,image_jwt_file)
        print( "  (saving as prov_cmd.jwt)" )
        crypto.dump_jwt( prov_cmd,"prov_cmd.jwt" )
    
        print( "Step 4. Device processes the provisioning command" )
        dev.process_provision_cmd(prov_cmd)
  
        print( "Protocol Complete; saving state and committing logs" )
        hsm.save_state()
        dev.save_state()
