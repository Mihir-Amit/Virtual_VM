#ifndef CODE_HPP
#define CODE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
using namespace std;

enum class ErrorCode { SUCCESS, NOT_FOUND, UNAUTHORIZED, INVALID_CONFIGURATION, SYSTEM_ERROR };
enum class VMState { INITIALIZED, RUNNING, PAUSED, STOPPED, ERROR };
enum class BillingStatus { PAID, PENDING, OVERDUE, SUSPENDED, CURRENT };

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

// Declare classes
class OSImageManagementModule {};
class VMOrchestrationModule {  };
class BillingModule {  };

#endif
