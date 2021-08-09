import torch
import torch.nn as nn
from torch.utils.data import DataLoader
import torchvision.transforms as T  
from dataset import MnistDataset
import torch.nn.functional as F
class Model(nn.Module):
    def __init__(self,layer_size,hidden_layer,activation_func):
        super(Model,self).__init__()
        if(hidden_layer==0):
            self.fc1 = nn.Linear(3*40*40,10)
        elif(hidden_layer==1):
            self.fc1 = nn.Linear(3*40*40,layer_size)
            self.fc2 = nn.Linear(layer_size,10)
        elif(hidden_layer==2):
            self.fc1 = nn.Linear(3*40*40,layer_size)
            self.fc2 = nn.Linear(layer_size,layer_size)
            self.fc3 = nn.Linear(layer_size,10)
        self.hidden_layer = hidden_layer
        self.act = activation_func
    def forward(self,x):
        if self.hidden_layer==0:
            x = x.view(x.size(0),-1)
            x=self.fc1(x)
            x=torch.log_softmax(x,dim=1)

        elif self.hidden_layer==1:
            x = x.view(x.size(0),-1)
            x=self.fc1(x)

            if self.act ==0:
                x = torch.sigmoid(x)
            elif self.act ==1:
                x = torch.tanh(x)
            elif self.act ==2:
                x= F.relu(x)

            x=self.fc2(x)
            x=torch.log_softmax(x,dim=1)

        elif self.hidden_layer==2:
            x = x.view(x.size(0),-1)
            x=self.fc1(x)

            if self.act ==0:
                x = torch.sigmoid(x)
            elif self.act ==1:
                x = torch.tanh(x)
            elif self.act ==2:
                x= F.relu(x)

            x=self.fc2(x)

            if self.act ==0:
                x = torch.sigmoid(x)
            elif self.act ==1:
                x = torch.tanh(x)
            elif self.act ==2:
                x= F.relu(x)

            x = self.fc3(x)
            x=torch.log_softmax(x,dim=1)
        return x

if __name__=='__main__':
    transforms = T.Compose([
        T.ToTensor(),
        T.Normalize((0.5,),(0.5,)),
    ])
    dataset = MnistDataset('data','train',transforms)
    dataLoader = DataLoader(dataset, batch_size=64,shuffle=True,num_workers=4)
    model = Model(512,2,2   )
    for images, labels in dataLoader:        
        pred = model(images)
        print(pred)
        exit()