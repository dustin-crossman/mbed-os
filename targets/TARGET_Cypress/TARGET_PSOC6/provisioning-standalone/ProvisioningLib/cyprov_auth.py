class authorization :    
    
    def create_die_id( fab,lot,wafer,ypos,xpos ) :
        id= {
            "fab" : fab,
            "lot" : lot,
            "wafer" : wafer,
            "ypos" : ypos,
            "xpos" : xpos
        }
        return id
     
    def auth_add_dev_list( auth , dev_list , is_exclude_list=False ) :
        """
            Adds an list of <dev_id> strings to an authorization object
        """
        if not "dev_id" in auth :
            auth.dev_id= {}
        if is_exclude_list :
            auth.dev_id.exclude= dev_list
        else :
            auth.dev_id.include= dev_list
        if "include" in auth.dev_id and "exclude" in auth.dev_id :
            print( "It does not make sense to have both an 'include' and 'exclude' device list in your authorization" )
        
    def auth_add_die_limit( auth , fab , lot , wafer , is_min=False ) :
        """
            Adds a serial number constraint for a particular fab
        """
        if not "die_id" in auth :
            auth.die_id= {}
        if not fab in auth.die_id :
            auth.die_id[fab]= {}
        c= { "lot" : lot , "wafer" : wafer }
        if is_min :
            auth.die_id[fab].min= c
        else :
            auth.die_id[fab].max= c
        
    def check_authorization( auth,dev_id,die_id ) :
        """
            Checks whether the HSM has authorization limitations on either:
            - serial number (min/max)
            - device ID (include/exclude)
            Note that serial numbers contain a fab_id, so multiple constraints are possible for multiple fabs
            Note that there are no time bound restrictions; devices have no sense of time (yet), so cannot enforce these
        """
        # check die_id (serial number) constraints
        if "die_id" in auth :
            if not die_id.fab in auth.die_id :
                # if no constraints are given for our fab, authorization is not granted
                return False
            else :
                constraint= auth.die_id[die_id.fab]
                if "min" in constraint :
                    m= constraint.min
                    if die_id.lot < m.lot :
                        return False
                    elif die_id.lot == m.lot and die_id.wafer < m.wafer :
                        return False
                if "max" in constraint :
                    m= constraint.max
                    if die_id.lot > m.lot :
                        return False
                    elif die_id.lot == m.lot and die_id.wafer > m.wafer :
                        return False

        # check dev_id constraints
        if "dev_id" in auth :
            if "include" in auth.dev_id :
                # dev_id must be in include list
                if not d in auth.dev_id.include :
                    return False
            else :
                # dev_id must not be in exclude list
                if d in auth.dev_id.exclude :
                    return False     
        return True