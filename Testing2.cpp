#include <iostream>
#include <cassert>
#include <vector>
#include <string>

#include "Coding.cpp"
using namespace std;
class UnitTestRunner {
public:
    
    void testOSImageManagementModule() {
        cout << "Running OSImageManagementModule Tests..." << endl;

        
        {
            OSImageManagementModule imageManager;
            OSImage* image = new OSImage{"Ubuntu", "20.04", "Linux"};
            assert(imageManager.addOSImage(image) == ErrorCode::SUCCESS);
            
           
            OSImage* loadedImage = imageManager.loadOSImage("Ubuntu");
            assert(loadedImage != nullptr);
            assert(loadedImage->version == "20.04");
            assert(loadedImage->osType == "Linux");
        }

        
        {
            OSImageManagementModule imageManager;
            OSImage* image1 = new OSImage{"CentOS", "8", "Linux"};
            OSImage* image2 = new OSImage{"CentOS", "8", "Linux"};
            
            assert(imageManager.addOSImage(image1) == ErrorCode::SUCCESS);
            assert(imageManager.addOSImage(image2) == ErrorCode::INVALID_CONFIGURATION);
        }

        
        {
            OSImageManagementModule imageManager;
            OSImage* image = new OSImage{"Debian", "11", "Linux"};
            
            imageManager.addOSImage(image);
            assert(imageManager.removeOSImage("Debian") == ErrorCode::SUCCESS);
            assert(imageManager.loadOSImage("Debian") == nullptr);
        }

     
        {
            OSImageManagementModule imageManager;
            OSImage* image = new OSImage{"Windows", "10", "Windows"};
            imageManager.addOSImage(image);

            OSImage* updatedImage = new OSImage{"Windows", "11", "Windows"};
            assert(imageManager.updateOSImage(updatedImage) == ErrorCode::SUCCESS);
            
            OSImage* loadedImage = imageManager.loadOSImage("Windows");
            assert(loadedImage != nullptr);
            assert(loadedImage->version == "11");
        }

        cout << "OSImageManagementModule Tests Passed!" << endl;
    }

    
    void testVMOrchestrationModule() {
        cout << "Running VMOrchestrationModule Tests..." << endl;

       
        OSImageManagementModule imageManager;
        OSImage* image = new OSImage{"TestOS", "1.0", "Linux"};
        imageManager.addOSImage(image);

        
        {
            VMOrchestrationModule vmOrchestrator(&imageManager);
            VMSpec* vm = vmOrchestrator.createVM("TestOS", 2, 4, 50);
            
            assert(vm != nullptr);
            assert(vm->osImage == "TestOS");
            assert(vm->cpuCores == 2);
            assert(vm->ramGB == 4);
            assert(vm->currentState == VMState::STOPPED);
        }

      
        {
            VMOrchestrationModule vmOrchestrator(&imageManager);
            VMSpec* vm = vmOrchestrator.createVM("TestOS", 2, 4, 50);
            assert(vm != nullptr);

         
            assert(vmOrchestrator.startVM(vm->id) == ErrorCode::SUCCESS);
            
          
            assert(vmOrchestrator.pauseVM(vm->id) == ErrorCode::SUCCESS);
            
            assert(vmOrchestrator.stopVM(vm->id) == ErrorCode::SUCCESS);

            assert(vmOrchestrator.startVM(vm->id) == ErrorCode::SUCCESS);
           
            assert(vmOrchestrator.restartVM(vm->id) == ErrorCode::SUCCESS);
            
          
            
            
          
            assert(vmOrchestrator.deleteVM(vm->id) == ErrorCode::SUCCESS);
        }

        cout << "VMOrchestrationModule Tests Passed!" << endl;
    }

   
    void testBillingModule() {
        cout << "Running BillingModule Tests..." << endl;

       
        {
            BillingModule billingModule;
            BillingRecord* record = billingModule.createBillingRecord("user1");
            
            assert(record != nullptr);
            assert(record->userId == "user1");
            assert(record->currentCharges == 0.0);
            assert(record->status == BillingStatus::CURRENT);
        }

       
        {
            OSImageManagementModule imageManager;
            OSImage* image = new OSImage{"TestOS", "1.0", "Linux"};
            imageManager.addOSImage(image);

            VMOrchestrationModule vmOrchestrator(&imageManager);
            VMSpec* vm = vmOrchestrator.createVM("TestOS", 2, 4, 50);

            BillingModule billingModule;
            billingModule.createBillingRecord("user2");
            
            assert(billingModule.updateBillingRecord("user2", vm) == ErrorCode::SUCCESS);
        }

       
        {
            BillingModule billingModule;
            billingModule.createBillingRecord("user3");
            
           
            OSImageManagementModule imageManager;
            OSImage* image = new OSImage{"TestOS", "1.0", "Linux"};
            imageManager.addOSImage(image);

            VMOrchestrationModule vmOrchestrator(&imageManager);
            VMSpec* vm = vmOrchestrator.createVM("TestOS", 2, 4, 50);
            
            billingModule.updateBillingRecord("user3", vm);
            
            
            assert(billingModule.applyPayment("user3", 10.0) == ErrorCode::SUCCESS);
        }

        cout << "BillingModule Tests Passed!" << endl;
    }

  
    void runAllTests() {
        cout << "Starting Unit Tests for Cloud System..." << endl;
        
        testOSImageManagementModule();
        testVMOrchestrationModule();
        testBillingModule();
        
        cout << "All Unit Tests Passed Successfully!" << endl;
    }
};

int main() {
    UnitTestRunner testRunner;
    
    try {
        testRunner.runAllTests();
    } catch (const int e) {
        cout << "Test failed"<< endl;
       
    }

    return 0;
}