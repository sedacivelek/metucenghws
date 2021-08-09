import torch
import torchvision.transforms as T  
from dataset import MnistDataset
from torch.utils.data import DataLoader
from model import Model
import torch.nn.functional as F
import numpy as np
import os 
import matplotlib.pyplot as plt
def train(model,optimizer,traindataLoader,validdataLoader,epochs,device,besthypeloss):
    train_loss_plot = []
    valid_loss_plot = []
    avg_train_losses = []
    avg_valid_losses = []
    best_val_loss = float('inf')
    best_success_rate = 0
    for epoch_index in range(epochs):
        model.train()
        valid_correct=0
        correct = 0
        for images,labels in traindataLoader:
            images = images.to(device)
            labels = labels.to(device)
            optimizer.zero_grad()
            pred = model(images)
            _,output = torch.max(pred,1)
            correct += torch.sum(output==labels)
            loss = F.nll_loss(pred,labels)
            loss.backward()
            optimizer.step()
            train_loss_plot.append(loss.item())
        model.eval()
        for valid_images,valid_labels in validdataLoader:
            valid_images = valid_images.to(device)
            valid_labels = valid_labels.to(device)
            valid_pred = model(valid_images)
            _,valid_output = torch.max(valid_pred,1)
            valid_correct += torch.sum(valid_output==valid_labels)
            valid_loss = F.nll_loss(valid_pred,valid_labels)
            valid_loss_plot.append(valid_loss.item())
        
        train_loss_avg = np.average(train_loss_plot)
        valid_loss_avg = np.average(valid_loss_plot)
        avg_train_losses.append(train_loss_avg)
        avg_valid_losses.append(valid_loss_avg)
        
        print(f'Epoch: {epoch_index+1}/{epochs} ' +f'Train Loss: {train_loss_avg:.4f} ' +f'Valid Loss: {valid_loss_avg:.4f}')
        print(f'Train accuracy: {correct.item()/24000.0} Validation Accuracy: {valid_correct.item()/6000.0}')


        train_loss_plot = []
        valid_loss_plot = []
        if valid_loss < best_val_loss:
            best_val_loss = valid_loss
            best_success = valid_correct
            if(best_val_loss<besthypeloss):
                torch.save(model.state_dict(),'model_state_dict')
    return model, avg_train_losses,avg_valid_losses,best_val_loss,best_success


def gridSearch(traindataLoader,validDataLoader,validSet,epochs,device):
    hidden_layers = [0,1,2]
    activation_functions =[0,1,2] #[sigmoid, tanh, relu]
    layer_size = [256,512,1024]
    learning_rate = [0.01,0.003,0.001,0.0003,0.0001,0.00003]
    besthyperparameter = (-1,-1,-1,-1)
    besthypeloss = float('inf')
    last_train_loss = []
    last_valid_loss = []
    with open ("valid_results.txt","w") as vf:
        for layer in hidden_layers:
            vf.write('\n')
            for ls in layer_size:
                vf.write('\n')
                for rate in learning_rate:
                    vf.write('\n')
                    if layer==0:
                        vf.write(f'Hidden layer: {layer} Activation func: {-1}  Layer size: {ls} Learning rate: {rate}\n')
                        model = Model(ls,layer,-1)
                        model.to(device)
                        optimizer = torch.optim.Adam(model.parameters(),lr=rate)
                        model, train_loss, valid_loss, best_val_loss,best_correct=train(model,optimizer,traindataLoader,validDataLoader,epochs,device,besthypeloss)
                        vf.write(f'SuccessRate: {best_correct.item()/len(validSet)} ValidLoss: {best_val_loss.item()}\n')
                        if(best_val_loss<besthypeloss):
                            besthypeloss=best_val_loss
                            besthyperparameter = (layer,-1,ls,rate)
                            last_train_loss = train_loss
                            last_valid_loss = valid_loss
                    else:
                        for func in activation_functions:
                            vf.write(f'Hidden layer: {layer} Activation func: {func}  Layer size: {ls} Learning rate: {rate}\n')
                            model = Model(ls,layer,func)
                            model.to(device)
                            optimizer = torch.optim.Adam(model.parameters(),lr=rate)
                            model, train_loss, valid_loss, best_val_loss,best_correct=train(model,optimizer,traindataLoader,validDataLoader,epochs,device,besthypeloss)
                            vf.write(f'SuccessRate: {best_correct.item()/len(validSet)} ValidLoss: {best_val_loss.item()}\n')
                            if(best_val_loss<besthypeloss):
                                besthypeloss=best_val_loss
                                besthyperparameter = (layer,func,ls,rate)
                                last_train_loss = train_loss
                                last_valid_loss = valid_loss
    return besthyperparameter

def main():
    use_cuda = False
    device = torch.device('cuda'if use_cuda else 'cpu')
    epochs = 50
    torch.manual_seed(123)

    transforms = T.Compose([
        T.ToTensor(),
        T.Normalize((0.5,),(0.5,)),
    ])
    trainSet = MnistDataset('data','train',transforms)
    testSet = MnistDataset('data','test',transforms)
    trainSet,validSet = torch.utils.data.random_split(trainSet,[24000,6000])
    traindataLoader = DataLoader(trainSet, batch_size=64,shuffle=True,num_workers=4)
    validDataLoader = DataLoader(validSet,batch_size=64,shuffle=True,num_workers=4)
    testDataLoader = DataLoader(testSet,batch_size=64,num_workers=4)

    #for grid search, in comment since it was used just for finding best hyperparameter
    #  
    #   
    #besthypepar = gridSearch(traindataLoader,validDataLoader,validSet,epochs,device)
    #print(besthypepar)

    
    besthypeloss = float('inf')
    #besthyperparameters: 2 hidden layer, 512 layer size, relu function, 0.001  learning rate
    lastModel = Model(512,2,2)
    
    lastModel.to(device)
    optimizer = torch.optim.Adam(lastModel.parameters(),lr=0.001)
    lastModel, train_loss, valid_loss, best_val_loss,best_correct=train(lastModel,optimizer,traindataLoader,validDataLoader,epochs,device,besthypeloss)
    
    #plot train and valid loss
    fig = plt.figure()
    plt.plot(train_loss, label='Training Loss')
    plt.plot(valid_loss,label='Validation Loss')
    plt.axvline(valid_loss.index(min(valid_loss)),color="r",label='Early Stop')
    plt.xlabel('epoch')
    plt.ylabel('loss')
    plt.legend()   
    plt.tight_layout()    
    fig.savefig('best_loss_plot.png')
    
    #test
    lastModel.load_state_dict(torch.load('model_state_dict'))
    lastModel.eval()

    index =0 
    test_results = []
    for test_images,test_labels in testDataLoader:
        pred = lastModel(test_images)
        _,output = torch.max(pred,1)
        for i in range(output.size(0)):
            head,tail = os.path.split(testSet.data[index][0])
            test_results.append(f'{tail} '+f'{output[i].item()}\n')
            index = index+1
    with open("test_labels.txt","w") as f:
        f.writelines(test_results)



if __name__=='__main__':
    main()