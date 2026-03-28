# ChatappFrontend
FrontEnd for ChatApp: Allows interaction with BackEnd
Project for COP3003 Spring2026
--- 
## Team Members
- Keila Lopez Sosa
- Kolby Goar
- Alberto Cuch
- James Mill
---
## Requirements
- C++ Compiler
- Qt6 Framework
- Build Tool
---
## Steps
1. Clone Repository: 
git clone https://github.com/alberinfo-2nd/ChatappFrontend.git
2. Open the project in your IDE
3. Build Project
4. Run the application
---
## Class Structure
**User (Base Class)**
- #username
- -public_key
- -strikeCount
- +getUsername()
- +reportUser() 

**AdminUser (Derived Class)**
- -password
- +authenticate()
- +disconnectUser()
---
## OPP Used
### Encapsulation
- Data members like adminPassword, public_key or username are kept private. Access controlled via public getter and setter methods and only available to each object. 
### Inheritance
-  AdminUser inherits from the User base class, sharing common traits such as username, while having unique behaviors such as passwords which are retrieved from a text file.  
### Polymorphism
- Implemented through virtual and override functions. I.e., login changes for normal and admin users, where admin users require a text file lookup for the password hash.  
## Resources