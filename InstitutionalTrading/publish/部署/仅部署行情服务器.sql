prompt
prompt Creating table BASE_BROKER
prompt ==========================
prompt
create table BASE_BROKER
(
  brokerid   INTEGER not null,
  brokercode VARCHAR2(31),
  company    VARCHAR2(64),
  type       INTEGER,
  account    VARCHAR2(21),
  password   VARCHAR2(21),
  deleteflag INTEGER
)
;
comment on table BASE_BROKER
  is '���͹�˾��Ϣ';
comment on column BASE_BROKER.type
  is '0������ ʹ��1������ʹ��';
comment on column BASE_BROKER.deleteflag
  is 'ɾ�����       0 ��û��ɾ�� 1���Ѿ�ɾ��';
alter table BASE_BROKER
  add constraint PK_BROKER primary key (BROKERID);

  
  
  
prompt
prompt Creating table BASE_SERVERADDRQUOT
prompt ==================================
prompt
create table BASE_SERVERADDRQUOT
(
  addrid     INTEGER not null,
  brokercode VARCHAR2(11),
  address    VARCHAR2(128),
  port       INTEGER,
  deleteflag INTEGER
)
;
comment on column BASE_SERVERADDRQUOT.deleteflag
  is '0 ��û��ɾ�� 1���Ѿ�ɾ��';
alter table BASE_SERVERADDRQUOT
  add constraint PK_SERVERADDRQUOT primary key (ADDRID);
