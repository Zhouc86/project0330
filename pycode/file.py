import os
import sys
import shutil 
if sys.platform=="linux":
    source="/home/zhanhc/webrtc-checkout/src"
    dest ="/home/zhanhc/EgoSln/webrtc/include/linux"
else:
    #source="D:/webrtc-m75/src"
    source="F:/Github/webrtc-checkout/src"
    dest="e:/EgoSln/webrtc/include/"
 
def subdir(srcpath,dstpath):
    files=os.listdir(srcpath)
    for file in files:
        a=os.path.join(srcpath,file)
        if(os.path.isfile(a)):
            if a.endswith(".h"):
                if os.path.exists(dstpath) ==False:
                   os.makedirs(dstpath)
                shutil.copyfile(a,os.path.join(dstpath,file))
            
        elif os.path.isdir(a):
            
            dst=os.path.join(dstpath,file)
            subdir(a,dst)


def main():
    dstpath=dest
    path=source
    files = os.listdir(source)
    for file in files:
        a=os.path.join(path,file)
        if(os.path.isfile(a)):
            if a.endswith(".h"):
                if os.path.exists(dstpath) ==False:
                    os.makedirs(dstpath)
                shutil.copyfile(a,os.path.join(dstpath,file))
        elif os.path.isdir(a):
            dst=os.path.join(dstpath,file)
            subdir(a,dst)



main()
#print(root)
#print(files)
#for dir in dirs:
#    paths=os.path.join(source,dir)
#    print(paths)
        


