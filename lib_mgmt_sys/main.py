from fastapi import FastAPI, HTTPException
import uvicorn

app = FastAPI()

books = {}
users = {}

@app.get("/")
def root():
    return {"welcome": "to lib_mgmt_sys"}

@app.post("/books/")
def add_book(book_id: int, title: str, author: str):
    books[book_id] = {"title": title, "author": author}
    return books[book_id]

@app.get("/books/{book_id}")
def get_book(book_id: int):
    if book_id in books:
        return books[book_id]
    else:
        raise HTTPException(status_code=404, detail="Book not found")

@app.delete("/books/{book_id}")
def delete_book(book_id: int):
    if book_id in books:
        del books[book_id]
        return {"detail": "Book deleted"}
    else:
        raise HTTPException(status_code=404, detail="Book not found")

@app.post("/users/")
def add_user(user_id: int, password: str, name: str):
    users[user_id] = {"name": name, "password": password, "borrowed_books": []}
    return users[user_id]

@app.post("/users/{user_id}/borrow/{book_id}")
def borrow_book(user_id: int, book_id: int):
    if user_id not in users:
        raise HTTPException(status_code=404, detail="User not found")
    if book_id not in books:
        raise HTTPException(status_code=404, detail="Book not found")
    
    users[user_id]["borrowed_books"].append(book_id)
    return {"detail": f"User {user_id} borrowed book {book_id}"}

@app.post("/users/{user_id}/return/{book_id}")
def return_book(user_id: int, book_id: int):
    if user_id not in users:
        raise HTTPException(status_code=404, detail="User not found")
    if book_id not in books:
        raise HTTPException(status_code=404, detail="Book not found")
    if book_id not in users[user_id]["borrowed_books"]:
        raise HTTPException(status_code=400, detail="Book not borrowed by user")
    
    users[user_id]["borrowed_books"].remove(book_id)
    return {"detail": f"User {user_id} returned book {book_id}"}

@app.get("/users/{user_id}/books/")
def get_borrowed_books(user_id: int):
    if user_id not in users:
        raise HTTPException(status_code=404, detail="User not found")
    
    borrowed_books = users[user_id]["borrowed_books"]
    return {"borrowed_books": borrowed_books}

if __name__ == "__main__":
    uvicorn.run(app, host="127.0.0.1", port=8000)
