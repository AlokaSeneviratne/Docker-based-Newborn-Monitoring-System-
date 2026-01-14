from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import mysql.connector

app = FastAPI()

# ---------- CORS so frontend JS can call the API ----------
origins = [
    "https://sds-frontend-route-sdx-assignment-aloka-seneviratne.2.rahtiapp.fi",
    "https://sds-frontend-route-sdx-assignment-aloka-seneviratne.2.rahtiapp.fi/",
    "*",  
]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_methods=["*"],
    allow_headers=["*"],
)


# ----------- DATABASE CONNECTION -------------
def get_db():
    return mysql.connector.connect(
        host="mysql",
        user="root",
        password="password",
        database="sds_project",
        port=3306,
    )


# ------------ REQUEST MODEL ------------------
class SensorData(BaseModel):
    id: str
    hr: int
    hrValid: bool
    spo2: int
    spo2Valid: bool
    temp: float
    tempStatus: str
    motion: float
    motionStatus: str
    timestamp: int


# ------------ RESPONSE MODEL -----------------
class ReadingOut(BaseModel):
    id: int
    device_id: str
    hr: int
    hrValid: bool
    spo2: int
    spo2Valid: bool
    temp: float
    tempStatus: str
    motion: float
    motionStatus: str
    ts: int


# ------------ API ENDPOINT: RECEIVE DATA -------------------
@app.post("/api/data")
def receive_data(data: SensorData):
    db = get_db()
    cursor = db.cursor()

    sql = """
        INSERT INTO readings
        (device_id, hr, hrValid, spo2, spo2Valid, temp, tempStatus, motion, motionStatus, ts)
        VALUES (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)
    """

    cursor.execute(
        sql,
        (
            data.id,
            data.hr,
            data.hrValid,
            data.spo2,
            data.spo2Valid,
            data.temp,
            data.tempStatus,
            data.motion,
            data.motionStatus,
            data.timestamp,
        ),
    )

    db.commit()
    cursor.close()
    db.close()

    return {"status": "ok"}


# ------------ API ENDPOINT: LATEST READING -----------------
@app.get("/api/latest", response_model=ReadingOut)
def get_latest():
    db = get_db()
    cursor = db.cursor(dictionary=True)

    cursor.execute(
        """
        SELECT id, device_id, hr, hrValid, spo2, spo2Valid,
               temp, tempStatus, motion, motionStatus, ts
        FROM readings
        ORDER BY id DESC
        LIMIT 1
        """
    )
    row = cursor.fetchone()

    cursor.close()
    db.close()

    if not row:
        raise HTTPException(status_code=404, detail="No readings yet")

    return row


# ------------ API ENDPOINT: HISTORY -------------
@app.get("/api/history")
def get_history(limit: int = 100):
    db = get_db()
    cursor = db.cursor(dictionary=True)

    cursor.execute(
        """
        SELECT id, device_id, hr, hrValid, spo2, spo2Valid,
               temp, tempStatus, motion, motionStatus, ts
        FROM readings
        ORDER BY ts DESC
        LIMIT %s
        """,
        (limit,),
    )
    rows = cursor.fetchall()

    cursor.close()
    db.close()

    # return newest first is fine; frontend can reverse if needed
    return rows
