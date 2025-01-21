/*

			Functions:
				SetPlayerWaze(playerid, Float:x, Float:y, Float:z, color = 0x8A44E4FF)
				StopWazeGPS(playerid)
				IsValidWazeGPS(playerid)


*/


#if !defined _gps_included
	#include <GPS>
#endif

#if defined include_waze_gps
	#endinput
#endif

#define include_waze_gps

			// Definers
/*MAX_WAZE_DOTS = Max 1024(Samp Limit) - You can increase the size of the route display on the map according to
the amount of GangZones on your server: 1024 - MY_TOTAL_CREATED_GZS = The maximum amount you can increase.*/
#define 	MAX_WAZE_DOTS 			(100)
#define 	WAZE_UPDATE_TIME 		(1000)

			// Forrwards
forward UpdateWaze(playerid, Float:X, Float:Y, Float:Z);
forward OnPlayerWazeRouters(Path:pathid, playerid);

static enum WAZEGPS_ENUM {
	wazeGPS_Timer,
	wazeGPS_Color,
	wazeGPS_Count,

	// Rotas
	bool:wazeGPS_Created[MAX_WAZE_DOTS],
    wazeGPS_Routes[MAX_WAZE_DOTS],

	Float:wazeGPS_Position[3],
	Float:wazeGPS_TickPosition[3]
};

static 
	wazeGPS[MAX_PLAYERS][WAZEGPS_ENUM];

public OnPlayerDisconnect(playerid, reason) {

	WazeGPS_Reset(playerid);

    #if defined alsOnPlayerDisconnect
        return alsOnPlayerDisconnect(playerid, reason);
    #else
   	 	return true;
	#endif
}

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect alsOnPlayerDisconnect
#if defined alsOnPlayerDisconnect
    forward alsOnPlayerDisconnect(playerid, reason);
#endif



/*

					ooooooooo.   ooooo     ooo oooooooooo.  ooooo        ooooo   .oooooo.   
					`888   `Y88. `888'     `8' `888'   `Y8b `888'        `888'  d8P'  `Y8b  
					888   .d88'  888       8   888     888  888          888  888          
					888ooo88P'   888       8   888oooo888'  888          888  888          
					888          888       8   888    `88b  888          888  888          
					888          `88.    .8'   888    .88P  888       o  888  `88b    ooo  
					o888o           `YbodP'    o888bood8P'  o888ooooood8 o888o  `Y8bood8P'  

*/

public UpdateWaze(playerid, Float:X, Float:Y, Float:Z) {

	if(GetPlayerInterior(playerid) != 0) 
		return StopWazeGPS(playerid);
	
	if(IsPlayerInRangeOfPoint(playerid, 30.0, wazeGPS[playerid][wazeGPS_Position][0], wazeGPS[playerid][wazeGPS_Position][1], wazeGPS[playerid][wazeGPS_Position][2])) {
		StopWazeGPS(playerid);
	    return true;		// Prevent Lags
	}

	new
		MapNode:start,
		MapNode:target;

	GetPlayerPos(playerid, wazeGPS[playerid][wazeGPS_TickPosition][0], wazeGPS[playerid][wazeGPS_TickPosition][1], wazeGPS[playerid][wazeGPS_TickPosition][2]);
    if(GetClosestMapNodeToPoint(wazeGPS[playerid][wazeGPS_TickPosition][0], wazeGPS[playerid][wazeGPS_TickPosition][1], wazeGPS[playerid][wazeGPS_TickPosition][2], start) != 0) return true;
    if(GetClosestMapNodeToPoint(X, Y, Z, target)) return true;
	if(FindPathThreaded(start, target, "OnPlayerWazeRouters", "i", playerid)) return true; 
    return true;
}

public OnPlayerWazeRouters(Path:pathid, playerid) {
    if(!IsValidPath(pathid)) return true;
	if(!wazeGPS[playerid][wazeGPS_Timer]) return true;

	new size;
	GetPathSize(pathid, size);
	if(size == 1) return StopWazeGPS(playerid);

	DestroyWazeRoutesGPS(playerid);

	new 
		MapNode:nodeid,
		Float:x,
		Float:y,
		Float:z,
		Float:length,
		index;

	GetPlayerPos(playerid, x, y, z);

	GetPathLength(pathid, length);
	GetClosestMapNodeToPoint(x, y, z, nodeid);
	GetMapNodePos(nodeid, x, y, z);

	new 
		_size = MAX_WAZE_DOTS,
		Float:X, 
		Float:Y, 
		Float:Z;
	
	if(MAX_WAZE_DOTS > size) _size = size;

	for(new i = 0; i < _size; i++) {
		GetPathNode(pathid, i, nodeid);
		GetPathNodeIndex(pathid, nodeid, index);
		GetMapNodePos(nodeid, X, Y, Z);

		if(i == index) 
			if(!CreateWazePointer(playerid, x, y, X, Y, wazeGPS[playerid][wazeGPS_Color]))
				break;
	
		x = X + 0.5;
		y = Y + 0.5;
	}
    return true;
}



/*

					.oooooo..o ooooooooooooo   .oooooo.     .oooooo.   oooo    oooo  .oooooo..o 
					d8P'    `Y8 8'   888   `8  d8P'  `Y8b   d8P'  `Y8b  `888   .8P'  d8P'    `Y8 
					Y88bo.           888      888      888 888           888  d8'    Y88bo.      
					`"Y8888o.       888      888      888 888           88888[       `"Y8888o.  
						`"Y88b      888      888      888 888           888`88b.         `"Y88b 
					oo     .d8P      888      `88b    d88' `88b    ooo   888  `88b.  oo     .d8P 
					8""88888P'      o888o      `Y8bood8P'   `Y8bood8P'  o888o  o888o 8""88888P'  

*/

static stock Float:GDBP(Float:X, Float:Y, Float:Z, Float: PointX, Float: PointY, Float: PointZ) 
	return floatsqroot(floatadd(floatadd(floatpower(floatsub(X, PointX), 2.0), floatpower(floatsub(Y, PointY), 2.0)), floatpower(floatsub(Z, PointZ), 2.0)));

stock IsValidWazeGPS(playerid) 
	return (wazeGPS[playerid][wazeGPS_Timer] <= 0 ? (false) : (true));

static stock WazeGPS_Reset(playerid) {

    StopWazeGPS(playerid);

	static const e_Clear[WAZEGPS_ENUM];
	wazeGPS[playerid] = e_Clear;
}

stock SetPlayerWaze(playerid, Float:x, Float:y, Float:z, color = 0x8A44E4FF) {
	DestroyWazeRoutesGPS(playerid);		// Destroy Routes if exists.

	wazeGPS[playerid][wazeGPS_Color] = color;
	wazeGPS[playerid][wazeGPS_Position][0] = x;
	wazeGPS[playerid][wazeGPS_Position][1] = y;
	wazeGPS[playerid][wazeGPS_Position][2] = z;

	if(!wazeGPS[playerid][wazeGPS_Timer]) 
		wazeGPS[playerid][wazeGPS_Timer] = SetTimerEx("UpdateWaze", WAZE_UPDATE_TIME, true, "ifff", playerid, x, y, z);

	UpdateWaze(playerid, x, y, z);		// Force First Update
    return true;
}

stock StopWazeGPS(playerid) {

	if(wazeGPS[playerid][wazeGPS_Timer] != 0)
	    KillTimer(wazeGPS[playerid][wazeGPS_Timer]);
	wazeGPS[playerid][wazeGPS_Timer] = 0;

	DestroyWazeRoutesGPS(playerid);
    return true;
}

static stock DestroyWazeRoutesGPS(playerid) {

	for(new i = 0; i < wazeGPS[playerid][wazeGPS_Count]; i++) {
		if(!wazeGPS[playerid][wazeGPS_Created][i]) continue;

		wazeGPS[playerid][wazeGPS_Created][i] = false;
		GangZoneDestroy(wazeGPS[playerid][wazeGPS_Routes][i]);
	}

	wazeGPS[playerid][wazeGPS_Count] = 0;
}

static stock CreateWazePointer(playerid, Float:X1, Float:Y1, Float:X2, Float:Y2, color) {

	new 
        Float:Dis = 12.5,
        Float:x, 
		Float:y,
		Float:GDPointers = GDBP(X1, Y1, 0.0, X2, Y2, 0.0),
		Points = floatround(GDPointers / Dis),
		index;

	for(new i = 1; i <= Points; i++) {

		index = wazeGPS[playerid][wazeGPS_Count];

		if(index >= MAX_WAZE_DOTS) 
			return false;
		
        x = X1 + (((X2 - X1) / Points) * i);
		y = Y1 + (((Y2 - Y1) / Points) * i);

		wazeGPS[playerid][wazeGPS_Created][index] = true;
        wazeGPS[playerid][wazeGPS_Routes][index] = GangZoneCreate(x-(Dis/2)-5, y-(Dis/2)-5, x+(Dis/2)+5, y+(Dis/2)+5); 
		GangZoneShowForPlayer(playerid, wazeGPS[playerid][wazeGPS_Routes][index], color);

		wazeGPS[playerid][wazeGPS_Count]++;
	}
	return true;
}
