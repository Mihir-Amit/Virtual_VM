


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <ctime>
#include <algorithm>
using namespace std;
enum class ErrorCode {
    SUCCESS,
    NOT_FOUND,
    UNAUTHORIZED,
    INVALID_CONFIGURATION,
    SYSTEM_ERROR
};

// VM and System-related Enums
enum class VMState {
    INITIALIZED,
    RUNNING,
    PAUSED,
    STOPPED,
    ERROR
};

enum class BillingStatus {
    PAID,
    PENDING,
    OVERDUE,
    SUSPENDED,
    CURRENT
};


struct VMSpec {
     string id;
     string osImage;
    int cpuCores;
    int ramGB;
    int storageGB;
    VMState currentState;
};

struct OSImage {
     string imageName;
     string version;
     string osType;
};

struct BillingRecord {
     string userId;
    double currentCharges;
    double totalCharges;
    BillingStatus status;
    time_t billingPeriod;
};



class OSImageManagementModule {
private:
      map<  string, OSImage*> imageRepository;

public:
 
    ~OSImageManagementModule() {
        for (auto& pair : imageRepository) {
            delete pair.second;
        }
        imageRepository.clear();
    }

    ErrorCode addOSImage(OSImage* image) {
        if (image == nullptr) {
            return ErrorCode::INVALID_CONFIGURATION;
        }

        if (imageRepository.find(image->imageName) != imageRepository.end()) {
            return ErrorCode::INVALID_CONFIGURATION;
        }

        if (image->imageName.empty() || 
            image->version.empty() || 
            image->osType.empty()) {
            return ErrorCode::INVALID_CONFIGURATION;
        }
        imageRepository[image->imageName] = image;
        return ErrorCode::SUCCESS;
    }


    OSImage* loadOSImage(const   string& imageName) {
        auto it = imageRepository.find(imageName);
        return (it != imageRepository.end()) ? it->second : nullptr;
    }

  
    ErrorCode removeOSImage(const   string& imageName) {
     
        auto it = imageRepository.find(imageName);
        
       
        if (it == imageRepository.end()) {
            return ErrorCode::NOT_FOUND;
        }

        delete it->second;
        imageRepository.erase(it);   
        return ErrorCode::SUCCESS;
    }


    ErrorCode updateOSImage(OSImage* updatedImage) {
        if (updatedImage == nullptr) {
            return ErrorCode::INVALID_CONFIGURATION;
        }


        auto it = imageRepository.find(updatedImage->imageName);
        

        if (it == imageRepository.end()) {
            return ErrorCode::NOT_FOUND;
        }

       
        if (updatedImage->imageName.empty() || 
            updatedImage->version.empty() || 
            updatedImage->osType.empty()) {
            return ErrorCode::INVALID_CONFIGURATION;
        }

        delete it->second; 
        imageRepository[updatedImage->imageName] = updatedImage;
        
        return ErrorCode::SUCCESS;
    }

      vector<OSImage*> listAvailableImages() {
        vector<OSImage*> availableImages;
        for (const auto& pair : imageRepository) {
            availableImages.push_back(pair.second);
        }
        
        return availableImages;
    }

   
};

class VMOrchestrationModule {
private:
    map<string, VMSpec*> vmInventory;
    OSImageManagementModule* osImageManager;

public:
    VMOrchestrationModule(OSImageManagementModule* osImageManager) : osImageManager(osImageManager) {}

    ~VMOrchestrationModule() {
        for (auto& pair : vmInventory) {
            delete pair.second;
        }
        vmInventory.clear();
    }

  
    VMSpec* createVM(const string& osImageName, int cpuCores, int ramGB, int storageGB) {
        OSImage* osImage = osImageManager->loadOSImage(osImageName);
        if (!osImage) {
            return nullptr; 
        }

      
        string vmId = "VM-" + to_string(time(nullptr)) + "-" + to_string(rand());

        VMSpec* newVM = new VMSpec{
            vmId, osImageName, cpuCores, ramGB, storageGB, VMState::STOPPED
        };

        vmInventory[vmId] = newVM;
        return newVM;
    }

    ErrorCode startVM(const string& vmId) {
        auto it = vmInventory.find(vmId);
        if (it == vmInventory.end()) {
            return ErrorCode::NOT_FOUND;
        }

        VMSpec* vm = it->second;
        if (vm->currentState == VMState::RUNNING) {
            return ErrorCode::INVALID_CONFIGURATION; 
        }

        vm->currentState = VMState::RUNNING;
        return ErrorCode::SUCCESS;
    }

 
    ErrorCode stopVM(const string& vmId) {
        auto it = vmInventory.find(vmId);
        if (it == vmInventory.end()) {
            return ErrorCode::NOT_FOUND;
        }

        VMSpec* vm = it->second;
        if (vm->currentState == VMState::STOPPED) {
            return ErrorCode::INVALID_CONFIGURATION; 
        }

        vm->currentState = VMState::STOPPED;
        return ErrorCode::SUCCESS;
    }


    ErrorCode pauseVM(const string& vmId) {
        auto it = vmInventory.find(vmId);
        if (it == vmInventory.end()) {
            return ErrorCode::NOT_FOUND;
        }

        VMSpec* vm = it->second;
        if (vm->currentState != VMState::RUNNING) {
            return ErrorCode::INVALID_CONFIGURATION;
        }

        vm->currentState = VMState::PAUSED;
        return ErrorCode::SUCCESS;
    }

  
    ErrorCode restartVM(const string& vmId) {
        auto it = vmInventory.find(vmId);
        if (it == vmInventory.end()) {
            return ErrorCode::NOT_FOUND;
        }

        VMSpec* vm = it->second;
        if (vm->currentState != VMState::RUNNING) {
            return ErrorCode::INVALID_CONFIGURATION;
        }

        vm->currentState = VMState::STOPPED;
        vm->currentState = VMState::RUNNING;
        return ErrorCode::SUCCESS;
    }


    ErrorCode deleteVM(const string& vmId) {
        auto it = vmInventory.find(vmId);
        if (it == vmInventory.end()) {
            return ErrorCode::NOT_FOUND;
        }

        delete it->second;
        vmInventory.erase(it);
        return ErrorCode::SUCCESS;
    }

    
    VMSpec* getVMDetails(const string& vmId) {
        auto it = vmInventory.find(vmId);
        return (it != vmInventory.end()) ? it->second : nullptr;
    }

    // List all VMs
    vector<VMSpec*> listAllVMs() {
        vector<VMSpec*> vms;
        for (const auto& pair : vmInventory) {
            vms.push_back(pair.second);
        }
        return vms;
    }
};


class BillingModule {
private:
    map<string, BillingRecord*> billingRecords;
    const double HOURLY_COMPUTE_RATE = 0.10;  
    const double STORAGE_RATE = 0.05;       
public:
 
    ~BillingModule() {
        for (auto& pair : billingRecords) {
            delete pair.second;
        }
        billingRecords.clear();
    }

  
    BillingRecord* createBillingRecord(const string& userId) {
        if (billingRecords.find(userId) != billingRecords.end()) {
            return nullptr; 
        }

        BillingRecord* newRecord = new BillingRecord{
            userId, 0.0, 0.0, BillingStatus::CURRENT, time(nullptr)
        };

        billingRecords[userId] = newRecord;
        return newRecord;
    }

   
    ErrorCode updateBillingRecord(const string& userId, VMSpec* vmUsage) {
        if (!vmUsage) {
            return ErrorCode::INVALID_CONFIGURATION;
        }

        auto it = billingRecords.find(userId);
        if (it == billingRecords.end()) {
            return ErrorCode::NOT_FOUND;
        }

        BillingRecord* record = it->second;
        double usageCost = calculateResourceCosts(vmUsage);
        record->currentCharges += usageCost;
        record->totalCharges += usageCost;

        return ErrorCode::SUCCESS;
    }

 
    BillingRecord* generateMonthlyInvoice(const string& userId) {
        auto it = billingRecords.find(userId);
        if (it == billingRecords.end()) {
            return nullptr; 
        }

        BillingRecord* record = it->second;
        record->currentCharges = 0.0;
        record->billingPeriod = time(nullptr);

        return record;
    }


    ErrorCode applyPayment(const string& userId, double amount) {
        auto it = billingRecords.find(userId);
        if (it == billingRecords.end()) {
            return ErrorCode::NOT_FOUND; 
        }

        BillingRecord* record = it->second;
        record->totalCharges -= amount;

        if (record->totalCharges <= 0) {
            record->totalCharges = 0;
            record->status = BillingStatus::CURRENT;
        }

        return ErrorCode::SUCCESS;
    }

    double calculateResourceCosts(VMSpec* vmSpec) {
        if (!vmSpec) return 0.0;

        double computeCost = vmSpec->cpuCores * HOURLY_COMPUTE_RATE;
        double storageCost = vmSpec->storageGB * STORAGE_RATE;

       
        return computeCost + storageCost;
    }
};
