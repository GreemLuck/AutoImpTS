CREATE TABLE CDREC (
    Dataset     TEXT    NOT NULL,
    Ticks       INT     NOT NULL,
    Tolerance   REAL    NOT NULL,
    Truncation  INT     NOT NULL,
    Max_iter    INT     NOT NULL,
    Runtime     INT     NOT NULL,
    Rmse        REAL    NOT NULL,
    PRIMARY KEY(Dataset, Ticks, Tolerance, Truncation, Max_iter)
);