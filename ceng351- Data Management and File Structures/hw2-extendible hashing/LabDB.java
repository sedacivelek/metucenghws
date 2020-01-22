package ceng.ceng351.labdb;

import java.util.Vector;
import java.util.*;
class Bucket{
    int depth, size;
    Vector<String> bucket;

    public Bucket(int depth,int size){
        this.depth = depth;
        this.size=size;

        this.bucket = new Vector<String>();
    }
    public int incDepth(){
        depth++;
        return depth;
    }
    public int decDepth(){
        depth--;
        return depth;
    }
    boolean insert(String studentID){
        Iterator p = bucket.iterator();
        int count=0;
        while(p.hasNext()){
            p.next();
            count++;
        }
        if(count==size){
            return false;
        }
        bucket.add(studentID);
        return true;
    }

    boolean remove(String studentID){
        if(bucket.contains(studentID)){
            bucket.remove(studentID);
            return true;
        }
        else return false;
    }
}



public class LabDB {
    int global_depth;
    int bucketsize;
    Vector<Bucket> directory;

    int hash(String id, Integer gd){
        String s = id;
        s= s.replace("e","");
        int number = Integer.parseInt(s);
        int key = number&((1<<gd)-1);
        return key;
    }
    int doubleIndex(int bn,int ld){
        return bn^(1<<(ld-1));
    }
    void expand(){
        for(int i=0; i< 1<<global_depth ; i++) {
            Bucket b = new Bucket(directory.get(i).depth, bucketsize);
            b.bucket = directory.get(i).bucket;
            directory.add(b);
        }
        global_depth++;
    }

    void reduce(){
        for(int i=0; i<directory.size();i++){
            if(directory.get(i).depth==global_depth){
                return;
            }
        }
        global_depth--;
        for(int i=0; i<1<<global_depth ;i++){
            directory.remove(directory.size()-1);
        }
        reduce();
    }

    void split(int bn){
        Vector<String> temp;
        int local_depth = directory.get(bn).incDepth();
        for(int i=0;i<1<<global_depth;i++){
            if(directory.get(i).bucket==directory.get(bn).bucket){
                directory.get(i).depth=local_depth;
            }
        }
        if(local_depth>global_depth){
            expand();
        }
        int doubled = doubleIndex(bn,local_depth);
        temp = new Vector<>(directory.get(bn).bucket);
        directory.get(doubled).bucket = new Bucket(local_depth,bucketsize).bucket;
        directory.get(doubled).depth = local_depth;
        directory.get(doubled).size = bucketsize;

        directory.get(bn).bucket.clear();
        int diff = 1 << local_depth;
        int dir = 1 << global_depth;
        for(int i = doubled-diff ; i >=0 ; i -= diff){
            directory.get(i).bucket = directory.get(doubled).bucket;
            directory.get(i).depth=directory.get(doubled).depth;
            
        }
        for(int i=doubled+diff;i<dir;i+=diff){
            directory.get(i).bucket = directory.get(doubled).bucket;
            directory.get(i).depth=directory.get(doubled).depth;
          
        }
        Iterator it = temp.iterator();
        for(int i=0; it.hasNext();i++,it.next()){
            enter(temp.get(i));
        }


    }
    void merge(int bn){
        int local_depth = directory.get(bn).depth;
        int doubled = doubleIndex(bn,local_depth);


        if(directory.get(doubled).depth == local_depth){
            directory.get(doubled).decDepth();
            directory.get(bn).bucket=directory.get(doubled).bucket;
            directory.get(bn).depth = directory.get(doubled).depth;
            

            for(int i=0; i<1<<global_depth; i++){
                if(directory.get(bn).bucket==directory.get(i).bucket){
                    directory.get(i).depth=directory.get(bn).depth;
                    
                }
            }


        }

    }




    public LabDB(int bucketSize) {
        this.global_depth=1;
        this.bucketsize=bucketSize;
        this.directory = new Vector<Bucket>();
        for(int i=0;i<(1<< global_depth);i++){
            Bucket b = new Bucket(global_depth,bucketSize);
            this.directory.add(b);
        }

    }

    public void enter(String studentID) {
        if(searchin(studentID)!=-1) return;
        int key = hash(studentID,global_depth);

        boolean inserted = directory.get(key).insert(studentID);
        if(inserted==false){
            split(key);
            enter(studentID);
        }

    }

    public void leave(String studentID) {
        int key = hash(studentID,global_depth);
        if(directory.get(key).remove(studentID)){
            if(directory.get(key).bucket.isEmpty() && directory.get(key).depth > 1){
                merge(key);
                while(directory.get(key).depth==directory.get(doubleIndex(key,directory.get(key).depth)).depth&&directory.get(doubleIndex(key,directory.get(key).depth)).bucket.isEmpty() && directory.get(key).bucket.isEmpty() && directory.get(key).depth>1){
                    merge(key);


                }



            }
            reduce();


        }



    }

    public String search(String studentID) {
        String ret="-1";
        for(int i=0;i<1<<global_depth;i++){
            if(directory.get(i).bucket.contains(studentID)){
                ret = toBinary(i,global_depth);
            }

        }
        return ret;
    }
    public int searchin(String studentID){
        int ret = -1;
        for(int i=0;i<1<<global_depth;i++){
            if(directory.get(i).bucket.contains(studentID)){
                ret = i;
            }

        }
        return ret;
    }

    String toBinary(int x, int len) {

        if (len > 0) {
            return String.format("%" + len + "s",
                    Integer.toBinaryString(x)).replaceAll(" ", "0");
        }

        return null;
    }

    public void printLab() {
        System.out.println("Global depth : "+global_depth);
        for(int i =0 ; i< 1<<global_depth;i++){
            System.out.print(toBinary(i,global_depth)+" : "+"[Local depth:" + directory.get(i).depth+"]");
            for(String s: directory.get(i).bucket){
                System.out.print("<"+s+">");
            }
            System.out.print("\n");
        }

    }
}





