#include "Coding.cpp"
#include <iostream>
using namespace std;

int main() {
    // Create OSImageManagementModule
    OSImageManagementModule osManager;

    // Add OS Images
    OSImage* os1 = new OSImage{"Ubuntu18.04", "18.04", "Linux"};
    OSImage* os2 = new OSImage{"Windows10", "10.0", "Windows"};
    osManager.addOSImage(os1);
    osManager.addOSImage(os2);

    cout << "Available OS Images:" << endl;
    for (auto img : osManager.listAvailableImages()) {
        cout << "Name: " << img->imageName << ", Version: " << img->version << endl;
    }


    VMOrchestrationModule vmManager(&osManager);


    VMSpec* vm1 = vmManager.createVM("Ubuntu18.04", 4, 16, 200);
    if (vm1) {
        cout << "VM Created: ID: " << vm1->id << ", OS: " << vm1->osImage << endl;
    }

    VMSpec* vm2 = vmManager.createVM("Windows10", 8, 32, 500);
    if (vm2) {
        cout << "VM Created: ID: " << vm2->id << ", OS: " << vm2->osImage << endl;
    }


    if (vm1) {
        vmManager.startVM(vm1->id);
        cout << "Started VM: " << vm1->id << endl;
    }

  
    cout << "All VMs:" << endl;
    for (auto vm : vmManager.listAllVMs()) {
        cout << "VM ID: " << vm->id << ", OS: " << vm->osImage << ", State: " << static_cast<int>(vm->currentState) << endl;
    }


    BillingModule billingManager;


    BillingRecord* bill1 = billingManager.createBillingRecord("User1");
    if (bill1) {
        cout << "Billing Record Created for User: " << bill1->userId << endl;
    }


    billingManager.updateBillingRecord("User1", vm1);


    BillingRecord* invoice = billingManager.generateMonthlyInvoice("User1");
    if (invoice) {
        cout << "Monthly Invoice for User: " << invoice->userId
             << ", Total Charges: " << invoice->totalCharges << endl;
    }

    // Clean up
    delete vm1;
    delete vm2;

    return 0;
}
