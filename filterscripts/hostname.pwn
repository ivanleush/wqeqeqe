#include <a_samp>
#include <a_mysql>

forward ServerName();
forward GetOwnerNickName(name[]);
forward PiarTextToAll(server_name[], site_server[], forum_server[]);
forward @CONNECTION_LOG_BASE(unix_time);
forward AddLOG(log_string[]);

#define SCMALL SendClientMessageToAll
#define SCM SendClientMessage

new hostname[][128] = {
    "hostname � ARIZONA TEST | TESTOVIY SERVER",
    "hostname � ARIZONA TEST | TESTOVIY SERVER",
    "hostname � ARIZONA TEST | TESTOVIY SERVER",
    "hostname � ARIZONA TEST | TESTOVIY SERVER"
};

new OwnerNickNames[][24] = {
    "tyler"
};

new MySQL:LOG_BASE, bool:WRITE_ACTION_SERVER = false, global_str[144];

#define SCMALLF(%0,%1,%2) format(global_str, 1024, %1,%2), SendClientMessageToAll(%0, global_str)

public OnFilterScriptInit()
{
    SetTimer("ServerName", 1000, true);
    return 1;
}

public ServerName() 
    SendRconCommand(hostname[random(sizeof(hostname))]);

public GetOwnerNickName(name[])
{
    for(new i; i < sizeof(OwnerNickNames); i++)
        if(!strcmp(name, OwnerNickNames[i], false))
            return true;

    return false;
}

public PiarTextToAll(server_name[], site_server[], forum_server[])
{
    SCMALLF(0xF6AB2FFF, "{F6AB2F}[!]{ffffff}=-=-=-=-=-=-=-=-=-=-=-=-=-=[ {F6AB2F}Arizona %s{FFFFFF} ]=-=-=-=-=-=-=-=-=-=-=-=-=-={F6AB2F}[!]{ffffff}", server_name);
    SendClientMessageToAll(-1, "  ");
    SendClientMessageToAll(-1, "          \t�������� ������� {F6AB2F}/bonus{FFFFFF}");
    SCMALLF(-1, "          \t��������� ����: {F6AB2F}%s", site_server);
    SCMALLF(-1, "          \t��� �����: {F6AB2F}%s", forum_server);
    SendClientMessageToAll(-1, "  ");
    SCMALLF(0xF6AB2FFF, "{F6AB2F}[!]{ffffff}=-=-=-=-=-=-=-=-=-=-=-=-=-=[ {F6AB2F}Arizona %s{FFFFFF} ]=-=-=-=-=-=-=-=-=-=-=-=-=-={F6AB2F}[!]{ffffff}", server_name);
}

public @CONNECTION_LOG_BASE(unix_time) 
{    
    if(WRITE_ACTION_SERVER)
    {
        new ms = GetTickCount();
    
        LOG_BASE = mysql_connect("51.91.215.125", "gs302654", "gs302654", "LpafILp98uug");
    
        if(!mysql_errno(LOG_BASE))
            mysql_set_charset("cp1251", LOG_BASE);

        printf("SkillHigh Log-DataBase was connection in: %d ms! Unix_time: (%d)", GetTickCount() - ms, unix_time);
    }

    return _:LOG_BASE;
}

public AddLOG(log_string[])
{
    if(WRITE_ACTION_SERVER)
        mysql_tquery(LOG_BASE, log_string);
}
