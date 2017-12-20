CREATE SEQUENCE "SEQ_ACCOUNTID";

CREATE SEQUENCE "SEQ_ADDRID";

CREATE SEQUENCE "SEQ_BROKERID";

CREATE SEQUENCE "SEQ_IDCARDTYPE";

CREATE SEQUENCE "SEQ_MODULEID";

CREATE SEQUENCE "SEQ_MSGID";

CREATE SEQUENCE "SEQ_PERMISSIONID";

CREATE SEQUENCE "SEQ_SERVERNO";

/*==============================================================*/
/* Table: "ACCOUNT"                                             */
/*==============================================================*/
CREATE TABLE "ACCOUNT"  (
   "ACCOUNTID"          INTEGER                         NOT NULL,
   "ACCOUNT"            VARCHAR(20)                     NOT NULL,
   "PASSWORD"           VARCHAR2(32)                    NOT NULL,
   "ACCOUNTTYPE"        INTEGER                         NOT NULL,
   "OWNER"              INTEGER                         NOT NULL,
   "NAME"               VARCHAR2(20),
   "CONTACTINFO"        VARCHAR2(100),
   "IDCARDTYPE"         INTEGER,
   "IDCARDNO"           VARCHAR2(20),
   "ACCOUNTSTATUS"      INTEGER                         NOT NULL,
   "LOGONSTATUS"        INTEGER,
   "VIEWPERMISSION1"    INTEGER,
   "VIEWPERMISSION2"    INTEGER,
   "PERMISSIONID"       INTEGER                         NOT NULL,
   "ENDDATE"            DATE,
   CONSTRAINT PK_ACCOUNT PRIMARY KEY ("ACCOUNTID")
);

COMMENT ON TABLE "ACCOUNT" IS
'�˺ű�';

COMMENT ON COLUMN "ACCOUNT"."ACCOUNTTYPE" IS
'�˺����� 0 �����˺� 1 �����˺�';

COMMENT ON COLUMN "ACCOUNT"."ACCOUNTSTATUS" IS
'��¼״̬��0 δ���� 1 ����';

/*==============================================================*/
/* Index: "INDEX_TRADEACCOUNT"                                  */
/*==============================================================*/
CREATE UNIQUE INDEX "INDEX_TRADEACCOUNT" ON "ACCOUNT" (
   "ACCOUNT" ASC
);

/*==============================================================*/
/* Table: "BROKERINFO"                                          */
/*==============================================================*/
CREATE TABLE "BROKERINFO"  (
   "BROKERID"           INTEGER                         NOT NULL,
   "BROKERCODE"         VARCHAR2(20)                    NOT NULL,
   "COMPANY"            VARCHAR2(64)                    NOT NULL,
   "SHAREWITHLOWERLEVER" INTEGER,
   "OWNER"              INTEGER,
   CONSTRAINT PK_BROKERINFO PRIMARY KEY ("BROKERID")
);

COMMENT ON TABLE "BROKERINFO" IS
'���͹�˾��Ϣ';

/*==============================================================*/
/* Index: "INDEX_OWNERBROKERCODE"                               */
/*==============================================================*/
CREATE UNIQUE INDEX "INDEX_OWNERBROKERCODE" ON "BROKERINFO" (
   "OWNER" ASC,
   "BROKERCODE" ASC
);

/*==============================================================*/
/* Table: "IDCARD"                                              */
/*==============================================================*/
CREATE TABLE "IDCARD"  (
   "IDCARDTYPE"         INTEGER                         NOT NULL,
   "IDCARDNAME"         VARCHAR2(20)                    NOT NULL,
   CONSTRAINT PK_IDCARD PRIMARY KEY ("IDCARDTYPE")
);

COMMENT ON TABLE "IDCARD" IS
'֤������';

/*==============================================================*/
/* Table: "MESSAGEINFO"                                         */
/*==============================================================*/
CREATE TABLE "MESSAGEINFO"  (
   "MESSAGEID"          INTEGER                         NOT NULL,
   "TITLE"              VARCHAR2(40)                    NOT NULL,
   "CONTENT"            VARCHAR2(2000)                  NOT NULL,
   "EXPIREDDATE"        DATE                            NOT NULL,
   "OWNER"              INTEGER                         NOT NULL,
   "TYPE"               INTEGER                         NOT NULL,
   CONSTRAINT PK_MESSAGEINFO PRIMARY KEY ("MESSAGEID")
);

COMMENT ON COLUMN "MESSAGEINFO"."TYPE" IS
'0 ���͸�����ǰ̨�˺� 1 ���͸�ĳ�������˺����������ǰ̨�˺�';

/*==============================================================*/
/* Table: "MESSAGETARGET"                                       */
/*==============================================================*/
CREATE TABLE "MESSAGETARGET"  (
   "ACCOUNTID"          INTEGER                         NOT NULL,
   "MESSAGEID"          INTEGER                         NOT NULL,
   "ACCOUNT"            VARCHAR2(20),
   CONSTRAINT PK_MESSAGETARGET PRIMARY KEY ("ACCOUNTID", "MESSAGEID")
);

/*==============================================================*/
/* Table: "MESSAGEWAITFORSEND"                                  */
/*==============================================================*/
CREATE TABLE "MESSAGEWAITFORSEND"  (
   "ACCOUNTID"          INTEGER                         NOT NULL,
   "MESSAGEID"          INTEGER                         NOT NULL,
   "ACCOUNT"            VARCHAR(20),
   CONSTRAINT PK_MESSAGEWAITFORSEND PRIMARY KEY ("ACCOUNTID", "MESSAGEID")
);

/*==============================================================*/
/* Table: "PERMISSION"                                          */
/*==============================================================*/
CREATE TABLE "PERMISSION"  (
   "PERMISSIONID"       INTEGER                         NOT NULL,
   "OWNER"              INTEGER                         NOT NULL,
   "NAME"               VARCHAR2(20)                    NOT NULL,
   "PERMISSIONTYPE"     INTEGER                         NOT NULL,
   "PERMISSION"         BLOB,
   CONSTRAINT PK_PERMISSION PRIMARY KEY ("PERMISSIONID")
);

/*==============================================================*/
/* Index: "INDEX_OWNERNAMEMODULE"                               */
/*==============================================================*/
CREATE UNIQUE INDEX "INDEX_OWNERNAMEMODULE" ON "PERMISSION" (
   "OWNER" ASC,
   "NAME" ASC
);

/*==============================================================*/
/* Table: "PERMISSIONMODULE"                                    */
/*==============================================================*/
CREATE TABLE "PERMISSIONMODULE"  (
   "MODULEID"           INTEGER                         NOT NULL,
   "MODULENAME"         VARCHAR2(20)                    NOT NULL,
   "COMMONOPERATORS"    INTEGER                         NOT NULL,
   "ADDITIONALOPERATORS" INTEGER                         NOT NULL,
   "MODULETYPE"         INTEGER                         NOT NULL,
   CONSTRAINT PK_PERMISSIONMODULE PRIMARY KEY ("MODULEID")
);

COMMENT ON TABLE "PERMISSIONMODULE" IS
'Ȩ�޹���ģ���';

COMMENT ON COLUMN "PERMISSIONMODULE"."COMMONOPERATORS" IS
'����ļ��ֲ�����ÿһ��������λ��ʾһ�ֲ���';

COMMENT ON COLUMN "PERMISSIONMODULE"."MODULETYPE" IS
'0 ��̨ģ�� 1 ǰ̨ģ��';

/*==============================================================*/
/* Table: "SERVERADDR"                                          */
/*==============================================================*/
CREATE TABLE "SERVERADDR"  (
   "ADDRID"             INTEGER                         NOT NULL,
   "SERVERNO"           INTEGER,
   "ADDRESS"            VARCHAR2(128),
   "PORT"               INTEGER,
   "ADDRTYPE"           INTEGER,
   CONSTRAINT PK_SERVERADDR PRIMARY KEY ("ADDRID")
);

COMMENT ON TABLE "SERVERADDR" IS
'��������ַ��';

/*==============================================================*/
/* Table: "SERVERGROUP"                                         */
/*==============================================================*/
CREATE TABLE "SERVERGROUP"  (
   "SERVERNO"           INTEGER                         NOT NULL,
   "SERVERNAME"         VARCHAR2(20)                    NOT NULL,
   "BROKERID"           INTEGER                         NOT NULL,
   "SHAREWITHLOWER"     INTEGER,
   CONSTRAINT PK_SERVERGROUP PRIMARY KEY ("SERVERNO")
);

COMMENT ON TABLE "SERVERGROUP" IS
'����������Ϣ';

/*==============================================================*/
/* Index: "INDEX_SERVERGROUP"                                   */
/*==============================================================*/
CREATE UNIQUE INDEX "INDEX_SERVERGROUP" ON "SERVERGROUP" (
   "BROKERID" ASC,
   "SERVERNAME" ASC
);

/*==============================================================*/
/* Table: "TRADEACCOUNT"                                        */
/*==============================================================*/
CREATE TABLE "TRADEACCOUNT"  (
   "TRADEACCOUNT"       VARCHAR2(32)                    NOT NULL,
   "PASSWORD"           VARCHAR2(32)                    NOT NULL,
   "OWNER"              INTEGER                         NOT NULL,
   "BROKERID"           INTEGER                         NOT NULL,
   "SERVERNO"           INTEGER,
   "NICKNAME"           VARCHAR2(20),
   ID                   INTEGER                         NOT NULL,
   "ROWNO"              INTEGER                         NOT NULL
);

/*==============================================================*/
/* Index: "INDEX_OWNER_ACCT"                                    */
/*==============================================================*/
CREATE UNIQUE INDEX "INDEX_OWNER_ACCT" ON "TRADEACCOUNT" (
   "TRADEACCOUNT" ASC,
   "OWNER" ASC
);

ALTER TABLE "ACCOUNT"
   ADD CONSTRAINT FK_ACCOUNT_REFERENCE_IDCARD FOREIGN KEY ("IDCARDTYPE")
      REFERENCES "IDCARD" ("IDCARDTYPE");

ALTER TABLE "ACCOUNT"
   ADD CONSTRAINT FK_ACCOUNT_REFERENCE_PERMISSI FOREIGN KEY ("PERMISSIONID")
      REFERENCES "PERMISSION" ("PERMISSIONID");

ALTER TABLE "BROKERINFO"
   ADD CONSTRAINT FK_BROKERIN_REFERENCE_ACCOUNT FOREIGN KEY ("OWNER")
      REFERENCES "ACCOUNT" ("ACCOUNTID")
      ON DELETE CASCADE;

ALTER TABLE "MESSAGEINFO"
   ADD CONSTRAINT FK_MESSAGEI_REFERENCE_ACCOUNT FOREIGN KEY ("OWNER")
      REFERENCES "ACCOUNT" ("ACCOUNTID")
      ON DELETE CASCADE;

ALTER TABLE "MESSAGETARGET"
   ADD CONSTRAINT FK_MESSAGET_REFERENCE_MESSAGEI FOREIGN KEY ("MESSAGEID")
      REFERENCES "MESSAGEINFO" ("MESSAGEID");

ALTER TABLE "MESSAGEWAITFORSEND"
   ADD CONSTRAINT FK_MESSAGEW_REFERENCE_MESSAGEI FOREIGN KEY ("MESSAGEID")
      REFERENCES "MESSAGEINFO" ("MESSAGEID")
      ON DELETE CASCADE;

ALTER TABLE "PERMISSION"
   ADD CONSTRAINT FK_PERMISSI_REFERENCE_ACCOUNT FOREIGN KEY ("OWNER")
      REFERENCES "ACCOUNT" ("ACCOUNTID")
      ON DELETE CASCADE;

ALTER TABLE "SERVERADDR"
   ADD CONSTRAINT FK_SERVERAD_REFERENCE_SERVERGR FOREIGN KEY ("SERVERNO")
      REFERENCES "SERVERGROUP" ("SERVERNO")
      ON DELETE CASCADE;

ALTER TABLE "SERVERGROUP"
   ADD CONSTRAINT FK_SERVERGR_REFERENCE_BROKERIN FOREIGN KEY ("BROKERID")
      REFERENCES "BROKERINFO" ("BROKERID")
      ON DELETE CASCADE;

ALTER TABLE "TRADEACCOUNT"
   ADD CONSTRAINT FK_TRADEACC_REFERENCE_ACCOUNT FOREIGN KEY ("OWNER")
      REFERENCES "ACCOUNT" ("ACCOUNTID")
      ON DELETE CASCADE;
