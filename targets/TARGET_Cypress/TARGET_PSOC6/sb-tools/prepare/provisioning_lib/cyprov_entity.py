import json

# Cypress Entity
class Entity :
    def __init__( self , state_name , audit_name ) :
        self.state_name= state_name
        self.state_loaded= False
        self.audit_name= audit_name
        Entity.load_state(self)
        
    def load_state( self ) :
        try :
            with open(self.state_name,"r+") as f :
                self.state= json.loads(f.read())
                f.close()
                self.state_loaded= True
        except FileNotFoundError:
            self.state= {}
            self.state_loaded= False
            
    def save_state( self ) :
        if not self.state_loaded :
            raise Exception( "Internal error - state not loaded" )
        with open(self.state_name,"w") as f :
            f.write( json.dumps( self.state,indent=4 ) )
            f.close()
        
    def append_audit_record( self,record ) :
        with open(self.audit_name,"a") as f :
            f.write( json.dumps(record,indent=4) + "\n" )
            f.close()
