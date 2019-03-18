from cyprov_entity import Entity
from cyprov_crypto import crypto

# Customer Entity
class CustomerEntity(Entity):
    def __init__(self, state_name, audit_name) :
        Entity.__init__(self, state_name, audit_name)
        
    def create_entity(self, kid) :
        """
        Creates the Customer entity.
        Creates the Customer main key-pair and returns nothing.
        """
        customer_priv_key, customer_pub_key = crypto.create_jwk()
        customer_priv_key["kid"] = str(kid)
        customer_pub_key["kid"] = str(kid)
        self.state["custom_priv_key"] = customer_priv_key
        self.state["custom_pub_key"] = customer_pub_key
        self.state_loaded = True
    
    def get_pub_key(self):
        return self.state["custom_pub_key"]
    
    def get_priv_key(self):
        return self.state["custom_priv_key"]
    