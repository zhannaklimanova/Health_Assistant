/*
================================================================
Name              :HealthAssistant.cpp
Author            :Zhanna Klimanova
Description       :A program that with users to calculate
                   their body fat percentage and propose a
                   personalized calories breakdown intake,
                   aiding them in their journey to improve their
                   health. Body fat percentage is calculated
                   via two methods: USNavy and BMI.
                   The program is implemented with the
                   object oriented paradigm. Multiple classes
                   are used to ensure a clear separation of
                   responsibilities for different aspects of the
                   program. Exception handling, file reading and
                   writing, as well as user statistics
                   calculations are some of the notable features
                   of the program.
================================================================
*/

/* -- Includes -- */
#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <vector>
#include <optional>
#include <memory>

/* -- Functions -- */

void bufferClear();
std::string center(const std::string &input, int width);
std::string double_to_string(double input, int precision);
std::string toLower(const std::string &input);
std::string trim(const std::string &str);

/* -- Global Fields -- */
enum class BfpType { BmiMethod, USNavyMethod };

/**
 * @struct UserInfo
 * @brief Represents information about a user.
 */
struct UserInfo {
    int age = 0;                           ///< Age of the user.
    double weight = 0.0;                   ///< Weight of the user in kilograms.
    double waist = 0.0;                    ///< Waist circumference of the user in centimeters.
    double neck = 0.0;                     ///< Neck circumference of the user in centimeters.
    double height = 0.0;                   ///< Height of the user in centimeters.
    double carbs = 0.0;                    ///< Daily carbohydrate intake of the user in grams.
    double protein = 0.0;                  ///< Daily protein intake of the user in grams.
    double fat = 0.0;                      ///< Daily fat intake of the user in grams.
    std::pair<int, std::string> bfp;       ///< Body Fat Percentage (BFP) as a pair of percentage and category.
    int daily_calories = 0;                ///< Daily caloric intake of the user.
    double hip = 0.0;                      ///< Hip circumference of the user in centimeters.
    std::string name = "";                 ///< Name of the user.
    std::string gender = "";               ///< Gender of the user.
    std::string lifestyle = "";            ///< Lifestyle category of the user.
};

/* -- Classes -- */

/**
 * @class UserInfoManager
 * @brief Manages user information using a linked list.
 *
 * This class provides functionality to manage user information through a linked list.
 * It includes methods for adding, deleting, reading from/writing to files, and displaying
 * user information.
 */
class UserInfoManager
{
    public:
        void addUserInfo(); // adds info to list
        void deleteUser(std::string username); // removes a user
        void readFromFile(std::string filename); // read and populate list
        void writeToFile(std::string filename);
        void display(std::string username);
        void displayAll();

        // Utilities
        UserInfo *getUserInfo(std::string username);
        void addUserInfo(UserInfo *userInfo);

    private:
        std::vector<UserInfo*> userInfoList;
        void displayUser(UserInfo *userInfo);

        // Commandline user input
        void getGender(UserInfo *user);
        void getHipMeasurement(UserInfo *user);
        void getAge(UserInfo *user);
        void getBodyWeightMeasurement(UserInfo *user);
        void getWaistMeasurement(UserInfo *user);
        void getNeckMeasurement(UserInfo *user);
        void getHeightMeasurement(UserInfo *user);
        void getLifestyle(UserInfo *user);
        void getName(UserInfo *user);
    };

/**
 * @class HealthAssistant
 * @brief Manages health-related functionalities by wrapping around UserInfoManager.
 *
 * This class acts as a wrapper around UserInfoManager, providing simplified methods
 * for managing user health information. It includes methods for getting user details,
 * Body Fat Percentage (BFP), Daily Calories, Meal Preparation details, and more. It also
 * acts as a bridge to UserInfoManager for operations like serialization, reading from files,
 * deleting users, and mass loading and computing.
 */
class HealthAssistant
{
    public:
        virtual ~HealthAssistant() {};
        void getUserDetails(); // wrapper method that simply calls addUserInfo in class UserInfoManager
        virtual void getBfp(std::string username) = 0;
        void getDailyCalories(std::string username);
        void getMealPrep(std::string username);
        void display(std::string username); // wrapper method
        void serialize(std::string filename); // wrapper method
        void readFromFile(std::string filename); // wrapper method
        void deleteUser(std::string username); // wrapper method
        void massLoadAndCompute(std::string filename);
    protected:
        static UserInfoManager userInfoManager;
    private:
        virtual void getBfp(UserInfo *user) = 0;
        void getDailyCalories(UserInfo *user);
        void getMealPrep(UserInfo *user);
};

// Initialize the static member variable userInfoManager
UserInfoManager HealthAssistant::userInfoManager;

/**
 * @brief The USNavyMethod class represents a health assistant that implements the US Navy method
 * for calculating body fat percentage (BFP).
 *
 * This class inherits from the HealthAssistant class and provides implementations for calculating
 * BFP using the US Navy method.
 */
class USNavyMethod : public HealthAssistant
{
    public:
        void getBfp(std::string username) override;
    private:
        void getBfp(UserInfo *user) override;
};

/**
 * @brief The BmiMethod class represents a health assistant that implements the BMI method
 * for calculating body fat percentage (BFP).
 *
 * This class inherits from the HealthAssistant class and provides implementations for calculating
 * BFP using the BMI method.
 */
class BmiMethod : public HealthAssistant
{
    public:
        void getBfp(std::string username) override;
    private:
        void getBfp(UserInfo *user) override;
};

/**
 * @brief The UserStats class represents a utility class for managing and retrieving user statistics.
 *
 * This class provides methods to retrieve information about healthy and unfit users based on different
 * criteria and methods, as well as to compute full statistics for all users.
 */
class UserStats
{
    public:
        std::vector<std::string> GetHealthyUsers(std::string method, std::string gender);
        std::vector<std::string> GetHealthyUsers(std::string method);
        std::vector<std::string> GetUnfitUsers(std::string method, std::string gender);
        std::vector<std::string> GetUnfitUsers(std::string method);
        void GetFullStats();
    private:
        std::shared_ptr<std::vector<UserInfo*>> massLoadAndCompute(std::string filename, BfpType bfpType);
        void usNavyMethod(UserInfo *user);
        void bmiMethod(UserInfo *user);
        void getDailyCalories(UserInfo *user);
        void getMealPrep(UserInfo *user);
};

/* -- Main Program -- */
int main(int argc, char* argv[])
{
    HealthAssistant* ha = new USNavyMethod();
    std::string userInput;
    // Loop until the user inputs "exit"
    while (true)
    {
        // Get user details
        ha->getUserDetails();
        // Ask for user input
        std::cout << "Enter 'exit' to quit, or press Enter to continue:";
        std::getline(std::cin, userInput);
        std::cout << std::endl;
        // Check if the user wants to exit
        if (userInput == "exit")
        {
            break; // exit the loop
        }
    }

    std::cout << "USNavyMethod before calculation" << std::endl;
    ha->display("all");
    ha->getBfp("john");
    ha->getDailyCalories("john");
    ha->getMealPrep("john");
    ha->serialize("us_user_data.csv");
    std::cout << "USNavyMethod after calculation" << std::endl;
    ha->display("all"); // only john's information is computed at this point
    delete ha;

    ha = new BmiMethod();
    // Loop until the user inputs "exit"
    while (true)
    {
        // Get user details
        ha->getUserDetails();
        // Ask for user input
        std::cout << "Enter 'exit' to quit, or press Enter to continue:";
        std::getline(std::cin, userInput);
        // Check if the user wants to exit
        if (userInput == "exit")
        {
            break; // Exit the loop
        }
    }

    std::cout << "BmiMethod before calculation" << std::endl;
    ha->display("all");
    ha->getBfp("john");
    ha->getDailyCalories("john");
    ha->getMealPrep("john");
    ha->deleteUser("jack"); // assuming we have a user jack
    std::cout << "BmiMethod after calculation" << std::endl;
    ha->display("all"); // should display all except for jack
    ha->serialize("bmi_user_data.csv");
    delete ha;

    ha = new USNavyMethod();
    ha->massLoadAndCompute("us_user_data.csv");
    std::cout << "USNavyMethod after massLoadAndCompute" << std::endl;
    ha->display("all");
    delete ha;
    ha = new BmiMethod();
    ha->massLoadAndCompute("bmi_user_data.csv");
    std::cout << "BmiMethod after massLoadAndCompute" << std::endl;
    ha->display("all");
    delete ha;

    UserStats stat;

    stat.GetHealthyUsers("bmi", "female");
    // stat.GetHealthyUsers("bmi", "male"); // extra test
    // stat.GetHealthyUsers("USArmy", "female"); // extra test
    // stat.GetHealthyUsers("USArmy", "male"); // extra test
    stat.GetHealthyUsers("all");

    // stat.GetUnfitUsers("bmi", "female"); // extra test
    // stat.GetUnfitUsers("bmi", "male"); // extra test
    // stat.GetUnfitUsers("USArmy", "female"); // extra test
    stat.GetUnfitUsers("USArmy", "male");
    // stat.GetUnfitUsers("all"); // extra test
    stat.GetFullStats();
}

/* -- Function Definitions -- */

/**
 * @brief Adds user information to the linked list maintained by UserInfoManager.
 *
 * This method creates a new UserInfo object, populates it with user details
 * through various helper methods, and adds it to the end of the linked list.
 * If the list is empty, the new UserInfo object becomes the first element;
 * otherwise, it is appended to the end of the existing list.
 */
void UserInfoManager::addUserInfo()
{
    UserInfo *userInfo = new UserInfo;

    getName(userInfo);
    getGender(userInfo);
    getHipMeasurement(userInfo);
    getAge(userInfo);
    getBodyWeightMeasurement(userInfo);
    getWaistMeasurement(userInfo);
    getNeckMeasurement(userInfo);
    getHeightMeasurement(userInfo);
    getLifestyle(userInfo);
    addUserInfo(userInfo);
}

/**
 * @brief Wrapper method to add user information using UserInfoManager.
 *
 * This method acts as a simple wrapper, calling the addUserInfo method in the
 * UserInfoManager instance associated with the HealthAssistant class.
 */
void HealthAssistant::getUserDetails()
{
    userInfoManager.addUserInfo();
}

/**
 * @brief Helper function for centering text within a given width
 *
 * @param input Title of the display information
 * @param width Width for centering
 * @return std::string
 */
std::string center(const std::string &input, int width)
{
    int inputLength = input.length();
    int diff = width - inputLength;
    int pad1 = diff / 2;
    int pad2 = diff - pad1;

    if (width < inputLength)
    {
        return input;
    }

    return std::string(pad1, ' ') + input + std::string(pad2, ' ');
}

/**
 * @brief Converts a double value to a string with fixed precision.
 *
 * @param input The double precision floating-point number to be converted into a string.
 * @return std::string A string representation of the input number, formatted to two decimal places.
 */
std::string double_to_string(double input, int precision)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << input;
    return ss.str();
}

/**
 * @brief Displays a summary of the user's profile.
 *
 * This function presents a comprehensive overview of the user's input data and calculated results,
 * including personal details, body measurements, lifestyle information, body fat percentage,
 * recommended daily caloric intake, and macronutrient needs. The output is formatted to enhance
 * readability and provide a clear understanding of the user's health and nutritional profile.
 */
void UserInfoManager::displayUser(UserInfo *userInfo)
{
    const int width = 60; // Adjust the width based on your output console or preference
    const int precision = 2;

    std::cout << center("--- USER PROFILE SUMMARY ---", width) << "\n\n";

    // Personal Details
    std::cout << center("Personal Details:", width) << "\n";
    std::cout << center("Name: " + userInfo->name, width) << "\n";
    std::cout << center("Gender: " + userInfo->gender, width) << "\n";
    std::cout << center("Age (years): " + std::to_string(userInfo->age), width) << "\n";
    std::cout << center("Height (cm): " + double_to_string(userInfo->height, precision), width) << "\n";
    if (userInfo->gender == "female")
    {
        std::cout << center("Hip (cm): " + double_to_string(userInfo->hip, precision), width) << "\n";
    }

    // Body Measurements
    std::cout << "\n" << center("Body Measurements:", width) << "\n";
    std::cout << center("Weight (kg): " + double_to_string(userInfo->weight, precision), width) << "\n";
    std::cout << center("Waist (cm): " + double_to_string(userInfo->waist, precision), width) << "\n";
    std::cout << center("Neck (cm): " + double_to_string(userInfo->neck, precision), width) << "\n";

    // Lifestyle
    std::cout << "\n" << center("Lifestyle:", width) << "\n";
    std::cout << center("Activity Level: " + userInfo->lifestyle, width) << "\n";

    // Health Metrics
    std::cout << "\n" << center("Health Metrics:", width) << "\n";
    std::cout << center("Body Fat Percentage: " + double_to_string(userInfo->bfp.first, precision) + "% (" + userInfo->bfp.second + ")", width) << "\n";
    std::cout << center("Daily Caloric Intake (calories): " + double_to_string(userInfo->daily_calories, precision), width) << "\n";

    // Macronutrient Breakdown
    std::cout << "\n" << center("Macronutrient Breakdown (grams):", width) << "\n";
    std::cout << center("Carbs: " + double_to_string(userInfo->carbs, precision) + "g", width) << "\n";
    std::cout << center("Protein: " + double_to_string(userInfo->protein, precision) + "g", width) << "\n";
    std::cout << center("Fat: " + double_to_string(userInfo->fat, precision) + "g", width) << "\n";

    std::cout << std::endl;
}

/**
 * @brief Deletes the user with the specified username.
 *
 * This function deletes the user with the specified username from the user information list.
 *
 * @param username The username of the user to be deleted.
 */
void UserInfoManager::deleteUser(std::string username)
{
    auto it = userInfoList.begin();
    while (it != userInfoList.end())
    {
        if ((*it)->name == username)
        {
            break;
        }
        it = std::next(it,1);
    }

    if (it != userInfoList.end())
    {
        userInfoList.erase(it);
    }
}

/**
 * @brief Displays information for a specific user based on the username.
 *
 * This method searches for the user with the given username in the linked list
 * and displays their information. If the user is not found, appropriate messages
 * are printed.
 *
 * @param username The username of the user to display information for.
 */
void UserInfoManager::display(std::string username)
{
    if (userInfoList.empty())
    {
        std::cout << "no user in list" << std::endl;
        return;
    }

    for (UserInfo* user : userInfoList)
    {
        if (user->name == username)
        {
            displayUser(user);
            return;
        }
    }

    std::cout << "user not found" << std::endl;
}

/**
 * @brief Wrapper method to delete a user using UserInfoManager.
 *
 * This method acts as a simple wrapper, printing a message and calling the
 * deleteUser method in the UserInfoManager instance associated with the
 * HealthAssistant class.
 *
 * @param username The username of the user to be deleted.
 */
void HealthAssistant::deleteUser(std::string username)
{
    std::cout << "Deleting User by the Name: " << username << std::endl;
    userInfoManager.deleteUser(username);
}

/**
 * @brief Wrapper method to display user information using UserInfoManager.
 *
 * This method acts as a simple wrapper, checking if the specified username is
 * "all" to display information for all users, or else calling the display method
 * for the specific user in the UserInfoManager instance associated with the
 * HealthAssistant class.
 *
 * @param username The username of the user to display information for, or "all" for all users.
 */
void HealthAssistant::display(std::string username)
{
    if (username == "all")
    {
        userInfoManager.displayAll();
    }
    else
    {
        userInfoManager.display(username);
    }
}

/**
 * @brief Displays information for all users stored in the manager.
 *
 * This function iterates through the list of users and displays information for each user.
 */
void UserInfoManager::displayAll()
{
    const int width = 60;
    std::cout << center("--- BEGIN ALL USER ---", width) << std::endl << std::endl;
    for (UserInfo* user : userInfoList)
        displayUser(user);
    std::cout << center("--- END ALL USER ---", width) << std::endl << std::endl;
}

/**
 * @brief Calculates the body fat percentage (BFP) for the user with the specified username.
 *
 * This function retrieves the user information by username and calculates the BFP using the US Navy method.
 *
 * @param username The username of the user for whom BFP needs to be calculated.
 */
void USNavyMethod::getBfp(std::string username)
{
    UserInfo *user = userInfoManager.getUserInfo(username);
    getBfp(user);
}

/**
 * @brief Calculates the body fat percentage (BFP) for the user with the specified username.
 *
 * This function retrieves the user information by username and calculates the BFP using the BMI method.
 *
 * @param username The username of the user for whom BFP needs to be calculated.
 */
void BmiMethod::getBfp(std::string username)
{
    UserInfo *user = userInfoManager.getUserInfo(username);
    getBfp(user);
}

/**
 * @brief Calculates the body fat percentage (BFP) for the specified user using the US Navy method.
 *
 * This function calculates the BFP for the specified user based on their gender, age, waist, hip, neck,
 * and height using the US Navy method.
 *
 * @param user A pointer to the UserInfo object containing the user's information.
 */
void USNavyMethod::getBfp(UserInfo *user)
{
    double bfp;
    std::string category;

    if (user->gender == "female")
    {
        bfp = (495.0 / (1.29579 - 0.35004 * log10(user->waist + user->hip - user->neck) + 0.22100 * log10(user->height))) - 450.0;

        if (user->age >= 20 && user->age <= 39)
        {
            if (bfp < 21)
            {
                category = "USNavy: Low";
            }
            else if (bfp < 33)
            {
                category = "USNavy: Normal";
            }
            else if (bfp < 39)
            {
                category = "USNavy: High";
            }
            else
            {
                category = "USNavy: Very High";
            }
        }
        else if (user->age >= 40 && user->age <= 59)
        {
            if (bfp < 23)
            {
                category = "USNavy: Low";
            }
            else if (bfp < 34)
            {
                category = "USNavy: Normal";
            }
            else if (bfp < 40)
            {
                category = "USNavy: High";
            }
            else
            {
                category = "USNavy: Very High";
            }
        }
        else if (user->age >= 60 && user->age <= 79)
        {
            if (bfp < 24)
            {
                category = "USNavy: Low";
            }
            else if (bfp < 36)
            {
                category = "USNavy: Normal";
            }
            else if (bfp < 42)
            {
                category = "USNavy: High";
            }
            else
            {
                category = "USNavy: Very High";
            }
        }
        else
        {
            std::cout << "The body fat category cannot be determined because you are outside of the permitted age range." << std::endl;
        }
    }
    else if (user->gender == "male")
    {
        bfp = (495.0 / (1.0324 - 0.19077 * log10(user->waist - user->neck) + 0.15456 * log10(user->height))) - 450.0;
        if (user->age >= 20 && user->age <= 39)
        {
            if (bfp < 8)
            {
                category = "USNavy: Low";
            }
            else if (bfp < 20)
            {
                category = "USNavy: Normal";
            }
            else if (bfp < 25)
            {
                category = "USNavy: High";
            }
            else
            {
                category = "USNavy: Very High";
            }
        }
        else if (user->age >= 40 && user->age <= 59)
        {
            if (bfp < 11)
            {
                category = "USNavy: Low";
            }
            else if (bfp < 22)
            {
                category = "USNavy: Normal";
            }
            else if (bfp < 28)
            {
                category = "USNavy: High";
            }
            else
            {
                category = "USNavy: Very High";
            }
        }
        else if (user->age >= 60 && user->age <= 79)
        {
            if (bfp < 13)
            {
                category = "USNavy: Low";
            }
            else if (bfp < 25)
            {
                category = "USNavy: Normal";
            }
            else if (bfp < 30)
            {
                category = "USNavy: High";
            }
            else
            {
                category = "USNavy: Very High";
            }
        }
        else
        {
            std::cout << "The body fat category cannot be determined because you are outside of the permitted age range." << std::endl;
        }
    }

    // Return Body Fat Percentage (BFP) and category as a pair
    user->bfp = std::make_pair(static_cast<int>(bfp), category);
}

/**
 * @brief Calculates the body fat percentage (BFP) for the specified user using the BMI method.
 *
 * This function calculates the BFP for the specified user based on their weight and height using the BMI method.
 *
 * @param user A pointer to the UserInfo object containing the user's information.
 */
void BmiMethod::getBfp(UserInfo *user)
{
    double bfp;
    std::string category;

    bfp = (user->weight*100*100)/(user->height*user->height);

    if (bfp < 18.5)
    {
        category = "Bmi: Low";
    }
    else if (bfp < 25)
    {
        category = "Bmi: Normal";
    }
    else if (bfp < 30)
    {
        category = "Bmi: High";
    }
    else
    {
        category = "Bmi: Very High";
    }

    // Return Body Fat Percentage (BFP) and category as a pair
    user->bfp = std::make_pair(static_cast<int>(bfp), category);
}

/**
 * @brief Wrapper method to calculate the daily calorie intake using UserInfoManager.
 *
 * This method acts as a simple wrapper, retrieving the UserInfo object for the specified username
 * and then calling the getDailyCalories method in the UserInfoManager instance, passing the UserInfo object as an argument.
 *
 * @param username The username of the user for whom the daily calorie intake is calculated.
 */
void HealthAssistant::getDailyCalories(std::string username)
{
    UserInfo* user = userInfoManager.getUserInfo(username);
    getDailyCalories(user);
}

/**
 * @brief Calculates the recommended daily caloric intake based on age, gender, and lifestyle.
 *
 * This function estimates the daily caloric intake needed for an individual based on their age, gender,
 * and lifestyle activity level. The caloric intake recommendations are differentiated by age ranges
 * and activity levels ('sedentary', 'moderate', 'active') for both male and female genders.
 * The calculated intake aims to maintain the individual's current weight, assuming their reported
 * activity level.
 *
 * The activity levels are defined as follows:
 * - 'Sedentary': little or no exercise
 * - 'Moderate': light exercise/sports 1-3 days a week
 * - 'Active': hard exercise/sports 3-5 days a week
 *
 * Age is considered in three brackets:
 * - 19-30 years
 * - 31-50 years
 * - Over 50 years
 *
 * The calorie recommendations are based on general guidelines and may need adjustments for
 * individual metabolism, health conditions, and specific weight goals.
 * @param user Pointer to the UserInfo object for whom the daily calorie intake is calculated.
 */
void HealthAssistant::getDailyCalories(UserInfo *user)
{
    int calories = 0;

    if (user->gender == "male")
    {
        if (user->age >= 19 && user->age <= 30)
        {
            if (user->lifestyle == "sedentary")
            {
                calories = 2400;
            }
            else if (user->lifestyle == "moderate")
            {
                calories = 2800;
            }
            else if (user->lifestyle == "active")
            {
                calories = 3000;
            }
        }
        else if (user->age >= 31 && user->age <= 50)
        {
            if (user->lifestyle == "sedentary")
            {
                calories = 2200;
            }
            else if (user->lifestyle == "moderate")
            {
                calories = 2600;
            }
            else if (user->lifestyle == "active")
            {
                calories = 3000;
            }
        }
        else if (user->age > 50)
        {
            if (user->lifestyle == "sedentary")
            {
                calories = 2000;
            }
            else if (user->lifestyle == "moderate")
            {
                calories = 2400;
            }
            else if (user->lifestyle == "active")
            {
                calories = 2800;
            }
        }
    }
    else if (user->gender == "female")
    {
        if (user->age >= 19 && user->age <= 30)
        {
            if (user->lifestyle == "sedentary")
            {
                calories = 2000;
            }
            else if (user->lifestyle == "moderate")
            {
                calories = 2200;
            }
            else if (user->lifestyle == "active")
            {
                calories = 2400;
            }
        }
        else if (user->age >= 31 && user->age <= 50)
        {
            if (user->lifestyle == "sedentary")
            {
                calories = 1800;
            }
            else if (user->lifestyle == "moderate")
            {
                calories = 2000;
            }
            else if (user->lifestyle == "active")
            {
                calories = 2200;
            }
        }
        else if (user->age > 50)
        {
            if (user->lifestyle == "sedentary")
            {
                calories = 1600;
            }
            else if (user->lifestyle == "moderate")
            {
                calories = 1800;
            }
            else if (user->lifestyle == "active")
            {
                calories = 2200;
            }
        }
    } else {
        std::cerr << "This is ackward, your intake calarie wasn't able to be processed" << std::endl;
        calories = 0;
    }

    user->daily_calories = calories;
}

/**
 * @brief Retrieves the UserInfo object for a specified username.
 *
 * This method traverses the linked list to find the UserInfo object corresponding to the given username.
 * If the username is not found or the linked list is empty, error messages are displayed, and nullptr is returned.
 *
 * @param username The username of the user for whom the UserInfo object is retrieved.
 * @return Pointer to the UserInfo object if found, otherwise nullptr.
 */
UserInfo *UserInfoManager::getUserInfo(std::string username)
{
    if (userInfoList.empty())
    {
        std::cerr << "no user in list" << std::endl;
        return nullptr;
    }

    for (UserInfo* user : userInfoList)
    {
        if (user->name == username)
        {
            return user;
        }
    }

    std::cerr << "user not found" << std::endl;
    return nullptr;

}

/**
 * @brief Wrapper method to calculate the recommended macronutrient distribution for meal preparation using UserInfoManager.
 *
 * This method acts as a simple wrapper, retrieving the UserInfo object for the specified username
 * and then calling the getMealPrep method in the UserInfoManager instance, passing the UserInfo object as an argument.
 *
 * @param username The username of the user for whom the macronutrient distribution is calculated.
 */
void HealthAssistant::getMealPrep(std::string username)
{
    UserInfo *user = userInfoManager.getUserInfo(username);
    getMealPrep(user);
}

/**
 * @brief Calculates the required amounts of macronutrients based on a set caloric intake, adhering to standard nutritional guidelines.
 *
 * This function assists in meal planning by breaking down the total daily caloric intake into the recommended proportions of macronutrients:
 * 50% from carbohydrates, 30% from protein, and 20% from fat. It utilizes the caloric values of 4 calories per gram for both carbohydrates
 * and protein, and 9 calories per gram for fat, to calculate the necessary grams of each macronutrient needed to meet the specified caloric intake.
 * The calculated amounts are then returned via reference parameters, allowing for direct integration into meal planning or dietary adjustments.
 *
 * The function supports customization of the caloric intake, making it a versatile tool for dietary planning across a range of nutritional needs
 * and goals. Whether for weight management, athletic performance, or general health, this function provides a foundational step in constructing
 * a balanced diet.
 * @param user Pointer to the UserInfo object for whom the meal prep is calculated.
 */
void HealthAssistant::getMealPrep(UserInfo *user)
{
    // Define the percentage of total calories for each macronutrient
    const double carbs_percentage = 0.50;
    const double protein_percentage = 0.30;
    const double fat_percentage = 0.20;

    // Calculate the calories for each macronutrient
    double carbs_calories = user->daily_calories * carbs_percentage;
    double protein_calories = user->daily_calories * protein_percentage;
    double fat_calories = user->daily_calories * fat_percentage;

    // Calculate the grams for each macronutrient
    user->carbs = carbs_calories / 4.0;
    user->protein = protein_calories / 4.0;
    user->fat = fat_calories / 9.0;
}

/**
 * @brief Serializes user data to a CSV file.
 *
 * Usage example:
 * serialize("userData.csv");
 *
 * CSV file example after multiple runs:
 * male,28,72,91,43,,172,sedentary
 * female,23,61,68,36,70,170,moderate
 *
 * @param filename The name (and path, if necessary) of the CSV file to which user data will be appended.
 */
void UserInfoManager::writeToFile(std::string filename)
{
    std::ofstream file;

    // Open the file in append mode so that previous data is not overwritten
    file.open(filename, std::ios_base::app); // using ios_base::app to append to file
    if (file.is_open())
    {
        for (UserInfo* user : userInfoList)
        {
            file << user->name << ","
                << user->gender << ","
                << user->age << ","
                << user->weight << ","
                << user->waist << ","
                << user->neck << ","
                << (user->gender == "female" ? double_to_string(user->hip, 1) : "") << ","
                << user->height << ","
                << user->lifestyle << std::endl; // std::endl to flush the stream
        }
    }
    else
    {
        std::cerr << "Error opening file: " << filename << std::endl;
    }

    // Close the file stream
    file.close();
}

/**
 * @brief Wrapper method to serialize user information to a file using UserInfoManager.
 *
 * This method acts as a simple wrapper, calling the writeToFile method in the UserInfoManager instance associated with the
 * HealthAssistant class, passing the specified filename as an argument.
 *
 * @param filename The name of the file to which user information is serialized.
 */
void HealthAssistant::serialize(std::string filename)
{
    userInfoManager.writeToFile(filename);
}

/**
 * @brief Reads user data from a specified CSV file and stores it in a vector.
 *
 * This function opens a CSV file with the given filename and reads its contents line by line,
 * parsing each line into a User object. Each line in the CSV file represents a single user's data,
 * structured in the following order: Gender, Age, Weight, Waist, Neck, Hip (optional for females),
 * Height, Lifestyle. The function handles the conditional presence of the hip measurement by checking
 * if the relevant token is empty and assigns a default value of 0.0 if it is absent, ensuring compatibility
 * with male user records where this measurement is not applicable.
 *
 * Assumptions:
 * - The CSV file is located in the same local folder as the program.
 * - The file format is consistent with the expected structure.
 *  - name,gender,age,weight,waist,neck,hip,height,lifestyle
 *  - no empty line except the last line which must be empty.
 *
 * @param filename The name of the CSV file from which to read user data.
 * @throws std::runtime_error if the file cannot be opened, or if the file is empty.
 */
void UserInfoManager::readFromFile(std::string filename)
{
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file as it may not exist or cannot be opened: " + filename);
        return;
    }

    std::ifstream ifs(filename, std::ios::ate); // std::ios::ate means open at end to check if file is empty
    if (ifs.tellg() == 0)
    {
        throw std::runtime_error("File is empty: " + filename);
    }

    while (getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;
        UserInfo *user = new UserInfo;

        getline(iss, user->name, ',');
        getline(iss, user->gender, ',');
        getline(iss, token, ',');
        user->age = std::stoi(token);
        getline(iss, token, ',');
        user->weight = std::stod(token);
        getline(iss, token, ',');
        user->waist = std::stod(token);
        getline(iss, token, ',');
        user->neck = std::stod(token);

        // Check for hip measurement
        getline(iss, token, ',');
        user->hip = token.empty() ? 0.0 : std::stod(token); // ok to convert if token is not empty

        getline(iss, token, ',');
        user->height = std::stod(token);
        getline(iss, user->lifestyle);

        userInfoList.push_back(user);
        std::cout << line << std::endl;
    }

    file.close();
}

/**
 * @brief Wrapper method to read user information from a file using UserInfoManager.
 *
 * This method acts as a simple wrapper, calling the readFromFile method in the UserInfoManager instance associated with the
 * HealthAssistant class, passing the specified filename as an argument.
 *
 * @param filename The name of the file from which user information is read.
 */
void HealthAssistant::readFromFile(std::string filename)
{
    userInfoManager.readFromFile(filename);
}

/**
 * @brief Helper function to remove trailing whitespaces.
 *
 * @param input String input to convert.
 * @return std::string
 */
std::string trim(const std::string &str)
{
    auto start = str.begin();
    auto end = str.end();

    while (start != str.end() && std::isspace(*start))
    {
        start++;
    }

    do
    {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

/**
 * @brief Helper function to convert a string to lowercase.
 *
 * @param input String input to convert.
 * @return std::string
 */
std::string toLower(const std::string &input)
{
    std::string output = input;

    std::transform(output.begin(), output.end(), output.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    return output;
}

/**
 * @brief Prompts for and sets the user's gender after validation.
 *
 * Continuously prompts the user to input their gender until "male" or "female" is provided.
 * Converts the input to lowercase for validation and assigns it to the User struct field upon success.
 * @param user Pointer to the UserInfo object for which the gender information is obtained and set.
 */
void UserInfoManager::getGender(UserInfo *user)
{
    std::string input;

    while (true)
    {
        std::cout << "Please specify your gender as either male or female: ";
        std::cin >> input;
        input = toLower(trim(input));
        bufferClear();

        if (input == "male" || input == "female")
        {
            user->gender = input;
            break;
        }
        else
        {
            std::cerr << "The gender entered is not yet supported. Please specify as either \"male\" or \"female\"." << std::endl;
        }
    }
}

/**
 * @brief Prompts the user for their hip measurement in centimeters and assigns it to the user object.
 *
 * This function is only applicable if the user's gender is set to "female". It continuously prompts the user
 * to enter their hip measurement until a valid numeric input is provided. The function checks for valid decimal
 * or whole number input. If the input is not a valid number, it displays an error message and prompts the user again.
 * Upon receiving a valid input, the measurement is assigned to the User struct field.
 *
 * @param user Pointer to the UserInfo object for which the Hip information is obtained and set.
 */
void UserInfoManager::getHipMeasurement(UserInfo *user)
{
    double input;

    if (user->gender != "female")
    {
        return;
    }
    while (true)
    {
        std::cout << "Enter your hip measurement in centimeters: ";

        if (std::cin >> input)
        { // if the input is successfully read as a number
            user->hip = input;
            // std::cout << input << std::endl;
            break;
        }
        else
        {
            std::cerr << "Invalid input for hip measurement. Please specify your measurement as a whole number or a decimal." << std::endl;
        }
        bufferClear();
    }
    bufferClear();
}

/**
 * @brief Prompts the user for their age and validates the input.
 *
 * Continuously prompts the user to enter their age until a valid integer is provided.
 * If the input is not a valid integer, it displays an error message and prompts the user again.
 * Upon receiving a valid input, the age is assigned to the User struct field.
 * @param user Pointer to the UserInfo object for which the Age information is obtained and set.
 */
void UserInfoManager::getAge(UserInfo *user)
{
    double input;

    while (true)
    {
        std::cout << "Enter your age: ";

        if (std::cin >> input)
        { // if the input is successfully read as a number

            // age limits BMI
            if (input < 20)
            {
                std::cerr << "You need to be older to use this tool" << std::endl;
            }
            else if (input > 79)
            {
                std::cerr << "You need to be younger to use this tool" << std::endl;
            }
            else{
                user->age = input;
                // std::cout << user.g_age << std::endl;
                break;
            }
        }
        else
        {
            std::cerr << "Invalid input for age. Please specify your age as a whole number." << std::endl;
        }
        bufferClear();
    }
    bufferClear();
}

/**
 * @brief Prompts the user for their body weight and validates the input.
 *
 * Continuously prompts the user to enter their body weight in kilograms until a valid number is provided.
 * The input can be a whole number or a decimal. If the input is not valid, it displays an error message and
 * prompts the user again. Upon receiving a valid input, the weight is assigned to the User struct field.
 * @param user Pointer to the UserInfo object for which the Weight information is obtained and set.
 */
void UserInfoManager::getBodyWeightMeasurement(UserInfo *user)
{
    double input;

    while (true)
    {
        std::cout << "Enter your body weight in kilograms: ";

        if (std::cin >> input)
        { // if the input is successfully read as a number
            user->weight = input;
            // std::cout << input << std::endl;
            break;
        }
        else
        {
            std::cerr << "Invalid input for body weight. Please specify your measurement as a whole number or a decimal." << std::endl;
        }
        bufferClear();
    }
    bufferClear();
}

/**
 * @brief Prompts the user for their waist measurement and validates the input.
 *
 * Continuously prompts the user to enter their waist measurement in centimeters until a valid number is provided.
 * The input can be a whole number or a decimal. If the input is not valid, it displays an error message and
 * prompts the user again. Upon receiving a valid input, the waist measurement is assigned to the User struct field.
 * @param user Pointer to the UserInfo object for which the Waist information is obtained and set.
 */
void UserInfoManager::getWaistMeasurement(UserInfo *user)
{
    double input;

    while (true)
    {
        std::cout << "Input your waist measurement in centimeters: ";

        if (std::cin >> input)
        { // if the input is successfully read as a number
            user->waist = input;
            // std::cout << input << std::endl;
            break;
        }
        else
        {
            std::cerr << "Invalid input for waist measurement. Please specify your measurement as a whole number or a decimal." << std::endl;
        }
        bufferClear();
    }
    bufferClear();
}

/**
 * @brief Prompts the user for their neck measurement and validates the input.
 *
 * Continuously prompts the user to enter their neck measurement in centimeters until a valid number is provided.
 * The input can be a whole number or a decimal. If the input is not valid, it displays an error message and
 * prompts the user again. Upon receiving a valid input, the neck measurement is assigned to the User struct field.
 * @param user Pointer to the UserInfo object for which the Neck information is obtained and set.
 */
void UserInfoManager::getNeckMeasurement(UserInfo *user)
{
    double input;

    while (true)
    {
        std::cout << "Provide your neck measurement in centimeters: ";
        if (std::cin >> input)
        { // if the input is successfully read as a number
            user->neck = input;
            // std::cout << input << std::endl;
            break;
        }
        else
        {
            std::cerr << "Invalid input for neck measurement. Please specify your measurement as a whole number or a decimal." << std::endl;
        }
        bufferClear();
    }
    bufferClear();
}

/**
 * @brief Prompts the user for their height and validates the input.
 *
 * This function continuously prompts the user to enter their height in centimeters until a valid numeric input is provided.
 * The input can be either a whole number or a decimal. If the input is not a valid number, the function displays an error message
 * and prompts the user again. Upon receiving a valid input, the height is assigned to the User struct field.
 * @param user Pointer to the UserInfo object for which the Height information is obtained and set.
 */
void UserInfoManager::getHeightMeasurement(UserInfo *user)
{
    double input;

    while (true)
    {
        std::cout << "Provide your height measurement in centimeters: ";
        if (std::cin >> input)
        { // if the input is successfully read as a number
            user->height = input;
            // std::cout << input << std::endl;
            break;
        }
        else
        {
            std::cerr << "Invalid input for height measurement. Please specify your measurement as a whole number or a decimal." << std::endl;
        }
        bufferClear();
    }
    bufferClear();
}

/**
 * @brief Prompts the user for their lifestyle choice and validates the input.
 *
 * Continuously prompts the user to specify their lifestyle as either "sedentary", "moderate", or "active".
 * The input is case-insensitive. If the input does not match any of the expected lifestyle choices, it displays an error message and
 * prompts the user again. Upon receiving a valid input, the lifestyle choice is assigned to the User struct field.
 * @param user Pointer to the UserInfo object for which the Lifestyle information is obtained and set.
 */
void UserInfoManager::getLifestyle(UserInfo *user)
{
    std::string input;

    while (true)
    {
        std::cout << "Provide information about your current lifestyle: sedentary, moderate (moderately active) or active: ";
        std::cin >> input;
        input = toLower(trim(input));
        bufferClear();

        if (input == "moderately")
        {
            input = "moderate";
        }

        if (input == "sedentary" || input == "moderate" || input == "active")
        {
            user->lifestyle = input;
            break;
        }
        else
        {
            std::cerr << "The lifestyle you entered is not yet supported. Please specify as either \"sedentary\", \"moderate\", or \"active\"." << std::endl;
        }
    }
}

/**
 * @brief Gets and sets the name information for a user.
 *
 * This method prompts the user to enter their name and sets the name attribute in the UserInfo object accordingly.
 *
 * @param user Pointer to the UserInfo object for which the name information is obtained and set.
 */
void UserInfoManager::getName(UserInfo *user)
{
    std::string input;
    std::cout << "What is your name: ";
    std::cin >> input;
    bufferClear();

    user->name = toLower(trim(input));
}

/**
 * @brief Mass loads user information from a file and computes additional attributes.
 *
 * This method reads user information from a CSV file, creates UserInfo objects, and adds them to the linked list.
 * It then calculates the Body Fat Percentage (BFP), daily calorie intake, and recommended macronutrient distribution for each user.
 *
 * @param filename The name of the file from which user information is mass-loaded.
 * @throws std::runtime_error if the file cannot be opened, or if the file is empty.
 */
void HealthAssistant::massLoadAndCompute(std::string filename)
{
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file as it may not exist or cannot be opened: " + filename);
        return;
    }

    std::ifstream ifs(filename, std::ios::ate); // std::ios::ate means open at end to check if file is empty
    if (ifs.tellg() == 0)
    {
        throw std::runtime_error("File is empty: " + filename);
    }

    while (getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;
        UserInfo *user = new UserInfo;

        getline(iss, user->name, ',');
        getline(iss, user->gender, ',');
        getline(iss, token, ',');
        user->age = std::stoi(token);
        getline(iss, token, ',');
        user->weight = std::stod(token);
        getline(iss, token, ',');
        user->waist = std::stod(token);
        getline(iss, token, ',');
        user->neck = std::stod(token);

        // Check for hip measurement
        getline(iss, token, ',');
        user->hip = token.empty() ? 0.0 : std::stod(token); // ok to convert if token is not empty

        getline(iss, token, ',');
        user->height = std::stod(token);
        getline(iss, user->lifestyle);

        getBfp(user);
        getDailyCalories(user);
        getMealPrep(user);

        userInfoManager.addUserInfo(user);
    }

    file.close();
}

/**
 * @brief Adds user information to the manager.
 *
 * This function adds the provided user information to the list of user information managed by the UserInfoManager.
 *
 * @param userInfo A pointer to the UserInfo object containing the user's information to be added.
 */
void UserInfoManager::addUserInfo(UserInfo *userInfo){
    userInfoList.push_back(userInfo);
}

/**
 * @brief Clears the input buffer.
 *
 * This function clears the input buffer of the standard input stream (std::cin).
 * It is useful for discarding any remaining characters in the input buffer, such as after
 * an invalid input or when switching between different types of input.
 */
void bufferClear()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * @brief Loads user information from a file and computes body fat percentage (BFP) for each user.
 *
 * This function reads user information from the specified file and computes the body fat percentage
 * (BFP) for each user using the specified method (BMI method or US Navy method). It returns a shared
 * pointer to a vector containing pointers to UserInfo objects, representing the loaded user information.
 *
 * @param filename The name of the file containing user information.
 * @param bfpType The type of method used to compute body fat percentage (BMI method or US Navy method).
 * @return A shared pointer to a vector containing pointers to UserInfo objects representing the loaded user information.
 * @throws std::runtime_error if the file cannot be opened, or if the file is empty.
 */
std::shared_ptr<std::vector<UserInfo*>> UserStats::massLoadAndCompute(std::string filename, BfpType bfpType)
{
    std::ifstream file(filename);
    std::string line;
    std::shared_ptr<std::vector<UserInfo*>> UserInfoList = std::make_shared<std::vector<UserInfo*>>();

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file as it may not exist or cannot be opened: " + filename);
    }

    std::ifstream ifs(filename, std::ios::ate); // std::ios::ate means open at end to check if file is empty
    if (ifs.tellg() == 0)
    {
        throw std::runtime_error("File is empty: " + filename);
    }

    while (getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;
        UserInfo *user = new UserInfo;

        getline(iss, user->name, ',');
        getline(iss, user->gender, ',');
        getline(iss, token, ',');
        user->age = std::stoi(token);
        getline(iss, token, ',');
        user->weight = std::stod(token);
        getline(iss, token, ',');
        user->waist = std::stod(token);
        getline(iss, token, ',');
        user->neck = std::stod(token);

        // Check for hip measurement
        getline(iss, token, ',');
        user->hip = token.empty() ? 0.0 : std::stod(token); // ok to convert if token is not empty

        getline(iss, token, ',');
        user->height = std::stod(token);
        getline(iss, user->lifestyle);


        if (bfpType == BfpType::BmiMethod)
        {
            bmiMethod(user);
        }
        else if (bfpType == BfpType::USNavyMethod)
        {
            usNavyMethod(user);
        }

        getDailyCalories(user);
        getMealPrep(user);

        UserInfoList->push_back(user);
    }

    file.close();
    return UserInfoList;
}

/**
 * @brief Calculates the body fat percentage (BFP) for a user using the US Navy method and assigns the result to the user object.
 *
 * This function calculates the body fat percentage for the specified user using the US Navy method.
 * The calculation is based on the user's gender, age, waist, hip, neck, and height.
 * If the user's age falls outside the permitted range, the function prints a message indicating that
 * the body fat category cannot be determined.
 *
 * @param user A pointer to the UserInfo object containing the user's information.
 */
void UserStats::usNavyMethod(UserInfo *user)
{
    double bfp;
    std::string category;

    if (user->gender == "female")
    {
        bfp = (495.0 / (1.29579 - 0.35004 * log10(user->waist + user->hip - user->neck) + 0.22100 * log10(user->height))) - 450.0;

        if (user->age >= 20 && user->age <= 39)
        {
            if (bfp < 21)
            {
                category = "USNavy: Low";
            }
            else if (bfp < 33)
            {
                category = "USNavy: Normal";
            }
            else if (bfp < 39)
            {
                category = "USNavy: High";
            }
            else
            {
                category = "USNavy: Very High";
            }
        }
        else if (user->age >= 40 && user->age <= 59)
        {
            if (bfp < 23)
            {
                category = "USNavy: Low";
            }
            else if (bfp < 34)
            {
                category = "USNavy: Normal";
            }
            else if (bfp < 40)
            {
                category = "USNavy: High";
            }
            else
            {
                category = "USNavy: Very High";
            }
        }
        else if (user->age >= 60 && user->age <= 79)
        {
            if (bfp < 24)
            {
                category = "USNavy: Low";
            }
            else if (bfp < 36)
            {
                category = "USNavy: Normal";
            }
            else if (bfp < 42)
            {
                category = "USNavy: High";
            }
            else
            {
                category = "USNavy: Very High";
            }
        }
        else
        {
            std::cout << "The body fat category cannot be determined because you are outside of the permitted age range." << std::endl;
        }
    }
    else if (user->gender == "male")
    {
        bfp = (495.0 / (1.0324 - 0.19077 * log10(user->waist - user->neck) + 0.15456 * log10(user->height))) - 450.0;
        if (user->age >= 20 && user->age <= 39)
        {
            if (bfp < 8)
            {
                category = "USNavy: Low";
            }
            else if (bfp < 20)
            {
                category = "USNavy: Normal";
            }
            else if (bfp < 25)
            {
                category = "USNavy: High";
            }
            else
            {
                category = "USNavy: Very High";
            }
        }
        else if (user->age >= 40 && user->age <= 59)
        {
            if (bfp < 11)
            {
                category = "USNavy: Low";
            }
            else if (bfp < 22)
            {
                category = "USNavy: Normal";
            }
            else if (bfp < 28)
            {
                category = "USNavy: High";
            }
            else
            {
                category = "USNavy: Very High";
            }
        }
        else if (user->age >= 60 && user->age <= 79)
        {
            if (bfp < 13)
            {
                category = "USNavy: Low";
            }
            else if (bfp < 25)
            {
                category = "USNavy: Normal";
            }
            else if (bfp < 30)
            {
                category = "USNavy: High";
            }
            else
            {
                category = "USNavy: Very High";
            }
        }
        else
        {
            std::cout << "The body fat category cannot be determined because you are outside of the permitted age range." << std::endl;
        }
    }

    // Return Body Fat Percentage (BFP) and category as a pair
    user->bfp = std::make_pair(static_cast<int>(bfp), category);
}

/**
 * @brief Calculates the body fat percentage (BFP) for a user using the BMI method and assigns the result to the user object.
 *
 * This function calculates the body fat percentage for the specified user using the BMI method.
 * The calculation is based on the user's weight and height.
 *
 * @param user A pointer to the UserInfo object containing the user's information.
 */
void UserStats::bmiMethod(UserInfo *user)
{
    double bfp;
    std::string category;

    bfp = (user->weight*100*100)/(user->height*user->height);

    if (bfp < 18.5)
    {
        category = "Bmi: Low";
    }
    else if (bfp < 25)
    {
        category = "Bmi: Normal";
    }
    else if (bfp < 30)
    {
        category = "Bmi: High";
    }
    else
    {
        category = "Bmi: Very High";
    }

    // Return Body Fat Percentage (BFP) and category as a pair
    user->bfp = std::make_pair(static_cast<int>(bfp), category);
}

/**
 * @brief Calculates the daily caloric intake for a user based on gender, age, and lifestyle.
 *
 * This function calculates the daily caloric intake for the specified user based on their gender,
 * age, and lifestyle (sedentary, moderate, or active). The calculation follows standard guidelines
 * for daily caloric needs according to gender, age group, and activity level.
 *
 * @param user A pointer to the UserInfo object containing the user's information.
 */
void UserStats::getDailyCalories(UserInfo *user)
{
    int calories = 0;

    if (user->gender == "male")
    {
        if (user->age >= 19 && user->age <= 30)
        {
            if (user->lifestyle == "sedentary")
            {
                calories = 2400;
            }
            else if (user->lifestyle == "moderate")
            {
                calories = 2800;
            }
            else if (user->lifestyle == "active")
            {
                calories = 3000;
            }
        }
        else if (user->age >= 31 && user->age <= 50)
        {
            if (user->lifestyle == "sedentary")
            {
                calories = 2200;
            }
            else if (user->lifestyle == "moderate")
            {
                calories = 2600;
            }
            else if (user->lifestyle == "active")
            {
                calories = 3000;
            }
        }
        else if (user->age > 50)
        {
            if (user->lifestyle == "sedentary")
            {
                calories = 2000;
            }
            else if (user->lifestyle == "moderate")
            {
                calories = 2400;
            }
            else if (user->lifestyle == "active")
            {
                calories = 2800;
            }
        }
    }
    else if (user->gender == "female")
    {
        if (user->age >= 19 && user->age <= 30)
        {
            if (user->lifestyle == "sedentary")
            {
                calories = 2000;
            }
            else if (user->lifestyle == "moderate")
            {
                calories = 2200;
            }
            else if (user->lifestyle == "active")
            {
                calories = 2400;
            }
        }
        else if (user->age >= 31 && user->age <= 50)
        {
            if (user->lifestyle == "sedentary")
            {
                calories = 1800;
            }
            else if (user->lifestyle == "moderate")
            {
                calories = 2000;
            }
            else if (user->lifestyle == "active")
            {
                calories = 2200;
            }
        }
        else if (user->age > 50)
        {
            if (user->lifestyle == "sedentary")
            {
                calories = 1600;
            }
            else if (user->lifestyle == "moderate")
            {
                calories = 1800;
            }
            else if (user->lifestyle == "active")
            {
                calories = 2200;
            }
        }
    } else {
        std::cerr << "This is ackward, your intake calarie wasn't able to be processed" << std::endl;
        calories = 0;
    }

    user->daily_calories = calories;
}

/**
 * @brief Calculates the macronutrient distribution for meal preparation based on daily caloric intake.
 *
 * This function calculates the distribution of macronutrients (carbohydrates, protein, and fat)
 * for meal preparation based on the user's daily caloric intake. It defines the percentage of total
 * calories for each macronutrient and then calculates the calories and grams for carbohydrates, protein,
 * and fat accordingly.
 *
 * @param user A pointer to the UserInfo object containing the user's information.
 */
void UserStats::getMealPrep(UserInfo *user)
{
    // Define the percentage of total calories for each macronutrient
    const double carbs_percentage = 0.50;
    const double protein_percentage = 0.30;
    const double fat_percentage = 0.20;

    // Calculate the calories for each macronutrient
    double carbs_calories = user->daily_calories * carbs_percentage;
    double protein_calories = user->daily_calories * protein_percentage;
    double fat_calories = user->daily_calories * fat_percentage;

    // Calculate the grams for each macronutrient
    user->carbs = carbs_calories / 4.0;
    user->protein = protein_calories / 4.0;
    user->fat = fat_calories / 9.0;
}

/**
 * @brief Retrieves the names of healthy users based on the specified method and gender.
 *
 * This function retrieves the names of healthy users based on the specified method (BMI or US Navy)
 * and gender. It loads user information from the appropriate data file and computes the necessary
 * health-related metrics. Then, it filters users based on their gender and body fat percentage category
 * (e.g., "Bmi: Normal" for the BMI method or "USNavy: Normal" for the US Navy method) and returns
 * a vector containing the names of healthy users.
 *
 * @param method The method used to compute body fat percentage (BMI or US Navy).
 * @param gender The gender of users to retrieve (male or female).
 * @return A vector containing the names of healthy users based on the specified method and gender.
 */
std::vector<std::string> UserStats::GetHealthyUsers(std::string method, std::string gender)
{
    std::vector<std::string> healthyUsers;
    std::shared_ptr<std::vector<UserInfo*>> userStats;
    if (method == "bmi")
    {
        userStats = massLoadAndCompute("bmi_user_data.csv", BfpType::BmiMethod);
        for (auto user: *userStats)
        {
            if (user->gender == gender && user->bfp.second == "Bmi: Normal")
            {
                healthyUsers.push_back(user->name);
            }
        }
    }
    else if (method == "USArmy")
    {
        userStats = massLoadAndCompute("us_user_data.csv", BfpType::USNavyMethod);
        for (auto user: *userStats)
        {
            if (user->gender == gender && user->bfp.second == "USNavy: Normal")
            {
                healthyUsers.push_back(user->name);
            }
        }
    }

    std::cout << "Healthy Users (" << gender << ", " << method << " method):" << std::endl;
    for (auto name : healthyUsers)
    {
        std::cout << name << std::endl;
    }

    return healthyUsers;
}

/**
 * @brief Retrieves the names of healthy users based on the specified method.
 *
 * This function retrieves the names of healthy users based on the specified method (BMI or US Navy).
 * It loads user information from the appropriate data files, computes the necessary health-related metrics,
 * and filters users based on their body fat percentage category (e.g., "Bmi: Normal" for the BMI method
 * or "USNavy: Normal" for the US Navy method). The function returns a vector containing the names of
 * healthy users regardless of gender.
 *
 * @param method The method used to compute body fat percentage (BMI or US Navy).
 * @return A vector containing the names of healthy users based on the specified method.
 */
std::vector<std::string> UserStats::GetHealthyUsers(std::string method)
{
    std::vector<std::string> healthyUsers;
    std::shared_ptr<std::vector<UserInfo*>> userStats;

    userStats = massLoadAndCompute("bmi_user_data.csv", BfpType::BmiMethod);
    for (auto user: *userStats)
    {
        if (user->bfp.second == "Bmi: Normal")
        {
            healthyUsers.push_back(user->name);
        }
    }
    userStats = massLoadAndCompute("us_user_data.csv", BfpType::USNavyMethod);
    for (auto user: *userStats)
    {
        if (user->bfp.second == "USNavy: Normal")
        {
            healthyUsers.push_back(user->name);
        }
    }

    std::cout << "All Healthy Users " << std::endl;
    for (auto name : healthyUsers)
    {
        std::cout << name << std::endl;
    }

    return healthyUsers;
}

/**
 * @brief Retrieves the names of unfit users based on the specified method and gender.
 *
 * This function retrieves the names of unfit users based on the specified method (BMI or US Navy)
 * and gender. It loads user information from the appropriate data file, computes the necessary
 * health-related metrics, and filters users based on their gender and body fat percentage category.
 * Unfit users are those whose body fat percentage category does not match the "Normal" category
 * for the specified method.
 *
 * @param method The method used to compute body fat percentage (BMI or US Navy).
 * @param gender The gender of users to retrieve (male or female).
 * @return A vector containing the names of unfit users based on the specified method and gender.
 */
std::vector<std::string> UserStats::GetUnfitUsers(std::string method, std::string gender)
{
    std::vector<std::string> healthyUsers;
    std::shared_ptr<std::vector<UserInfo*>> userStats;
    if (method == "bmi")
    {
        userStats = massLoadAndCompute("bmi_user_data.csv", BfpType::BmiMethod);
        for (auto user: *userStats)
        {
            if (user->gender == gender && user->bfp.second != "Bmi: Normal")
            {
                healthyUsers.push_back(user->name);
            }
        }
    }
    else if (method == "USArmy")
    {
        userStats = massLoadAndCompute("us_user_data.csv", BfpType::USNavyMethod);
        for (auto user: *userStats)
        {
            if (user->gender == gender && user->bfp.second != "USNavy: Normal")
            {
                healthyUsers.push_back(user->name);
            }
        }
    }

    std::cout << "Unfit Users (" << gender << ", " << method << " method):" << std::endl;
    for (auto name : healthyUsers)
    {
        std::cout << name << std::endl;
    }

    return healthyUsers;
}

/**
 * @brief Retrieves the names of unfit users based on the specified method.
 *
 * This function retrieves the names of unfit users based on the specified method (BMI or US Navy).
 * It loads user information from the appropriate data files, computes the necessary health-related metrics,
 * and filters users based on their body fat percentage category. Unfit users are those whose body fat percentage
 * category does not match the "Normal" category for the specified method.
 *
 * @param method The method used to compute body fat percentage (BMI or US Navy).
 * @return A vector containing the names of unfit users based on the specified method.
 */
std::vector<std::string> UserStats::GetUnfitUsers(std::string method)
{
    std::vector<std::string> healthyUsers;
    std::shared_ptr<std::vector<UserInfo*>> userStats;

    userStats = massLoadAndCompute("bmi_user_data.csv", BfpType::BmiMethod);
    for (auto user: *userStats)
    {
        if (user->bfp.second != "Bmi: Normal")
        {
            healthyUsers.push_back(user->name);
        }
    }
    userStats = massLoadAndCompute("us_user_data.csv", BfpType::USNavyMethod);
    for (auto user: *userStats)
    {
        if (user->bfp.second != "USNavy: Normal")
        {
            healthyUsers.push_back(user->name);
        }
    }

    std::cout << "All Unfit Users " << std::endl;
    for (auto name : healthyUsers)
    {
        std::cout << name << std::endl;
    }

    return healthyUsers;
}

/**
 * @brief Retrieves the full statistics regarding user health.
 *
 * This function computes and displays various statistics regarding user health,
 * including the total number of users, the percentage of male and female users,
 * and the percentage of users categorized as healthy based on BMI and US Navy methods.
 *
 * It loads user information from BMI and US Navy data files, computes the necessary health-related metrics,
 * and calculates statistics such as the total number of users, the percentage of male and female users,
 * and the percentage of users with healthy body fat percentage categories for both methods.
 *
 * The function prints these statistics to the console.
 */
void UserStats::GetFullStats()
{
    std::shared_ptr<std::vector<UserInfo*>> bmiUserStats;
    std::shared_ptr<std::vector<UserInfo*>> usUserStats;
    bmiUserStats = massLoadAndCompute("bmi_user_data.csv", BfpType::BmiMethod);
    usUserStats = massLoadAndCompute("us_user_data.csv", BfpType::BmiMethod);
    int maleCount = 0, femaleCount = 0;
    int healthyBmiCount = 0, healthyUsArmyCount = 0;
    int healthyMaleBmiCount = 0, healthyFemaleBmiCount = 0;
    int healthyMaleUsArmyCount = 0, healthyFemaleUsArmyCount = 0;

    // Total Users
    int totalUsers = bmiUserStats->size() + usUserStats->size();

    // Percentage of male users
    for (auto user: *bmiUserStats)
    {
        if (user->gender == "female")
        {
            femaleCount++;
        }
        else if (user->gender == "male")
        {
            maleCount++;
        }

        if (user->bfp.second == "Bmi: Normal")
        {
            healthyBmiCount++;
            if (user->gender == "female")
            {
                healthyFemaleBmiCount++;
            }
            else if (user->gender == "male")
            {
                healthyMaleBmiCount++;
            }
        }
    }
    for (auto user: *usUserStats)
    {
        if (user->gender == "female")
        {
            femaleCount++;
        }
        else if (user->gender == "male")
        {
            maleCount++;
        }

        if (user->bfp.second == "Bmi: Normal")
        {
            healthyUsArmyCount++;
            if (user->gender == "female")
            {
                healthyFemaleUsArmyCount++;
            }
            else if (user->gender == "male")
            {
                healthyMaleUsArmyCount++;
            }
        }
    }
    // std::cout << totalUsers << std::endl;
    // std::cout << femaleCount << std::endl;
    // std::cout << maleCount << std::endl;
    // std::cout << healthyBmiCount << std::endl;
    // std::cout << healthyFemaleBmiCount << std::endl;
    // std::cout << healthyMaleBmiCount << std::endl;
    // std::cout << healthyUsArmyCount << std::endl;
    // std::cout << healthyFemaleUsArmyCount << std::endl;
    // std::cout << healthyMaleUsArmyCount << std::endl;

    std::cout << "total users: " << totalUsers << std::endl;
    std::cout << "male/female percentage: " << maleCount*100/totalUsers << "% / " << femaleCount*100/totalUsers << "%" << std::endl;
    std::cout << "healty bmi: " << healthyBmiCount*100/bmiUserStats->size() << "%"<< std::endl;
    std::cout << "healty bmi male/female: " << healthyMaleBmiCount*100/bmiUserStats->size() << "% / "  << healthyFemaleBmiCount*100/bmiUserStats->size() << "%" << std::endl;
    std::cout << "healty us: " << healthyUsArmyCount*100/usUserStats->size() << "%"<< std::endl;
    std::cout << "healty us male/female: " << healthyMaleUsArmyCount*100/usUserStats->size() << "% / " << healthyFemaleUsArmyCount*100/usUserStats->size() << "%"<< std::endl;
}