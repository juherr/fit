package com.cmdev.fit.eg;
import fit.Fixture;

public class Directory extends Fixture {

    private String userId;
    private String fullName = "Steven E. Newton";
    private String newFullName;
    
    public Directory() {
        super();
    }
    
    public void userId(String id) {
        userId = id;
    }
    
    public void find() {
    }
    
    public void save() {
        fullName = newFullName;
    }
    
    public void setFullName(String fName) {
        newFullName = fName;
    }
    
    public boolean exists() {
        return true;
    }
    
    public String fullName() {
        return fullName;
    }

}