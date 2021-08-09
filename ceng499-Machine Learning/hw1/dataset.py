import os 
from torch.utils.data import Dataset, DataLoader
from PIL import Image
import torchvision.transforms as T  
class MnistDataset(Dataset):
    #read labels.txt 
    def __init__(self,dataset_path, split,transforms):
        image_paths = os.path.join(dataset_path,split)
        self.data = []
        with open(os.path.join(image_paths,'labels.txt'),'r') as f:
            if(split=='test'):
                for line in f:
                    image_name = line.replace('\n','')
                    image_path = os.path.join(image_paths,image_name)
                    image_label = int(-1)
                    self.data.append((image_path,image_label))
            else:
                for line in f:
                    image_name , image_label = line.split()
                    image_path = os.path.join(image_paths,image_name)
                    image_label = int(image_label)
                    self.data.append((image_path,image_label))

        self.transforms = transforms

    #used by dataloader
    def __len__(self):
        return len(self.data)

    ##read data in getitem
    def __getitem__(self, index):
        image_path = self.data[index][0]
        image_label = self.data[index][1]
        image = Image.open(image_path)
        image = self.transforms(image)
        return image, image_label

if __name__=='__main__':
    transforms = T.Compose([
        T.ToTensor(),
        T.Normalize((0.5,),(0.5,)),
    ])
    dataset = MnistDataset('data','test',transforms)
    dataLoader = DataLoader(dataset, batch_size=64,shuffle=True,num_workers=4)
    for images, labels in dataLoader:
        print(images.size())
        print(labels)
