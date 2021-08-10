package project.utility;

import project.parts.Arm;
import project.parts.Base;
import project.parts.Part;
import project.parts.logics.Builder;
import project.parts.logics.Fixer;
import project.parts.logics.Inspector;
import project.parts.logics.Supplier;
import project.parts.payloads.Camera;
import project.parts.payloads.Gripper;
import project.parts.payloads.MaintenanceKit;
import project.parts.payloads.Welder;

import java.util.Random;

import java.lang.reflect.Field;

public class Common
{
    public static Random random = new Random() ;

    public static synchronized Object get (Object object , String fieldName )
    {
        // TODO
        // This function retrieves (gets) the private field of an object by using reflection
        // In case the function needs to throw an exception, throw this: SmartFactoryException( "Failed: get!" )

        try {
            //create class obj from the object
            Class getClass = object.getClass();
            //create Field class object by fieldName
            Field field = getClass.getDeclaredField(fieldName);
            //set accessibility
            boolean accessibility = field.isAccessible();
            field.setAccessible(true);
            //get value
            Object result = field.get(object);
            //set default accessibility again
            field.setAccessible(accessibility);
            //return field value
            return result;
        }
        catch (Exception e){
            throw new SmartFactoryException( "Failed: get!" );
        }

    }

    public static synchronized void set ( Object object , String fieldName , Object value )
    {
        // TODO
        // This function modifies (sets) the private field of an object by using reflection
        // In case the function needs to throw an exception, throw this: SmartFactoryException( "Failed: set!" )
        try{
            //create class obj from the object
            Class getClass = object.getClass();
            //create Field class object by fieldName
            Field field = getClass.getDeclaredField(fieldName);
            //set accessibility
            Boolean accessibility = field.isAccessible();
            field.setAccessible(true);
            //set value
            field.set(object,value);
            //set default accessibility again
            field.setAccessible(accessibility);
        }
        catch (Exception e){
            throw new SmartFactoryException( "Failed: set!" );
        }


    }
    public static Part createPartFactory(String name){
        try{
            //configure packages for reflection
            String prefixedName = "";
            switch (name){
                case "Arm":
                    prefixedName = "project.parts.Arm";
                    break;
                case "Supplier":
                    prefixedName = "project.parts.logics.Supplier";
                    break;
                case "Builder":
                    prefixedName = "project.parts.logics.Builder";
                    break;
                case "Inspector":
                    prefixedName = "project.parts.logics.Inspector";
                    break;
                case "Fixer":
                    prefixedName = "project.parts.logics.Fixer";
                    break;
                case "Gripper":
                    prefixedName = "project.parts.payloads.Gripper";
                    break;
                case "Camera":
                    prefixedName = "project.parts.payloads.Camera";
                    break;
                case "Welder":
                    prefixedName = "project.parts.payloads.Welder";
                    break;
                case "MaintenanceKit":
                    prefixedName = "project.parts.payloads.MaintenanceKit";
                    break;
            }

            //create class obj from class name
            Class createPart = Class.forName(prefixedName);
            //create object according to name
            if(createPart.getName().equals("project.parts.Arm")) return new Arm();
            else if(createPart.getName().equals("project.parts.logics.Supplier")) return new Supplier();
            else if(createPart.getName().equals("project.parts.logics.Builder")) return new Builder();
            else if(createPart.getName().equals("project.parts.logics.Inspector")) return new Inspector();
            else if(createPart.getName().equals("project.parts.logics.Fixer")) return new Fixer();
            else if(createPart.getName().equals("project.parts.payloads.Gripper")) return new Gripper();
            else if(createPart.getName().equals("project.parts.payloads.Camera")) return new Camera();
            else if(createPart.getName().equals("project.parts.payloads.Welder")) return new Welder();
            else if(createPart.getName().equals("project.parts.payloads.MaintenanceKit")) return new MaintenanceKit();
        }
        catch (Exception e){
            throw new SmartFactoryException( "Failed: createPart!" );
        }
        return null;
    }
    public static Base createBaseFactory(int nextSerialNo){
        //create new base
        return new Base(nextSerialNo-1);
    }
}