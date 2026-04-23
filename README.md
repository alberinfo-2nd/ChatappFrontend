# ChatappFrontend

FrontEnd for ChatApp: Messaging interface built with C++ and Qt6.
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
- Build Tool: CMake or qmake

---

## Steps

1. Clone Repository:
   git clone https://github.com/alberinfo-2nd/ChatappFrontend.git
2. Open the project in your IDE
3. Build Project
4. Run the application

---

## Features

1. Secure Messaging: Encrypted messages only decrpytable with the recipient's private key which is stored only on their client. 
2. Message Cleaning: Self destroying messages that are deleted from the server's memory once they're read by the user.
3. Admin Management: Control system for admins to issue strikes and manage active user sessions.
4. User Reporting: Integrated reporting mechanism to flag problematic behavior.

---

## Class Structure

- MainWindow: Acts as the primary controller for switching between UI pages (Login, UserList, and Chat).
- SessionManager: Manages the logged-in user’s state, message inbox, and session-specific logic like reported users.
- ChatPage: The primary messaging interface. Updates based on signals from the SessionManager.
- BackendClient: Handles all network communication and API calls to the server.
- ActiveUsersManager: Tracks and updates the list of currently online users.

---

## OPP Used

### Encapsulation

- Data members like m_authorizationToken, m_reportedUsers, and m_isAdmin are kept private within SessionManager. Access controlled via public getter and setter methods.

### Inheritance

- Classes like ChatPage and LoginPage inherit from QWidget, allowing them to use Qt's specialized layout and event-handling systems.
- SessionManager, ActiverUsersManager, and BackendClient all inherit from QObject to allow signal and slot functionality.

### Composition

- SessionManager has-a User, which is the currently logged in user.
- ActiveUsersManager has-a vector User's, which is the list of every logged in user.
- BackendClient has-a pointer to SessionManager to access the logged in user's data, such as their username and authorization token.

---

## AI Tools

- Gemini: Used for debugging and troubleshooting UI implementation to help resolve layout issues within the Qt framework.
- ChatGPT: Used for support in the understanding of Qt syntax, assistance in debugging, and interpretation of Qt documentation.

---

## External Resources

- YouTube tutorial on basic Qt GUI setup: https://www.youtube.com/playlist?list=PLS1QulWo1RIZiBcTr5urECberTITj7gjA

---

### Collaboration Policy Statement

All submitted work reflects our team’s understanding. Any external code
has been:

- Reviewed
- Modified as needed
- Integrated by our team
