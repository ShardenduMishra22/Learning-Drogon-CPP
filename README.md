## Drogon C++ API – Auth Microservice (Docs)

### 🔧 Stack

* **C++17**
* **Drogon Web Framework**
* **JsonCpp**
* **jwt-cpp**
* **Fedora Linux (GCC 15.1.1)**

---

## 🏗️ Project Structure

```
Learning-Drogon-CPP/
├── main.cc                     → Drogon app entry point
├── config.json                 → Server config (port, threads, etc)
├── controllers/
│   └── AuthController.cc       → Handles POST /login (JWT issuance)
├── build/                      → CMake build output
├── views/, models/, plugins/  → (Default Drogon dirs)
```

---

## 🔐 Auth Flow

### Endpoint: `POST /login`

(Originally `/register`, you renamed or treat as login)

#### Payload:

```json
{
  "username": "admin",
  "password": "pass"
}
```

#### Response:

```json
{
  "token": "<JWT token>"
}
```

#### Logic:

* Validates hardcoded credentials
* If matched, returns JWT signed with `"secret"` key
* Else, returns 401 Unauthorized

---

## 🧪 Test (via curl)

```bash
curl -X POST http://localhost:5555/login \
  -H "Content-Type: application/json" \
  -d '{"username":"admin", "password":"pass"}'
```

---

## 🧱 Tech Concepts Used

* **Drogon App Bootstrapping**

  * `drogon::app().addListener("0.0.0.0", 5555);`
  * `drogon::app().run();`
* **JSON Parsing** (JsonCpp)
* **JWT Issuance** (jwt-cpp)
* **CMake-based build setup**
* **Postman / curl testing**

---

## 🧠 Lessons Learnt

* Drogon server doesn’t auto-load `controllers/` unless built and mapped in `PATH_ADD`
* JWT in C++ is low-level — manual parsing, claims, secret handling
* Drogon build works via `cmake .. && make`
* Default `config.json` can override programmatic port/listener
* `/register` path = login logic unless you add a new controller