create table register (
	heating boolean,
	roomtemp real,

	isdaytime boolean,
	humidity real,
	temp real,
	windDir real,
	windSpeed real,
	UV real,
	precipitation real,
	cloudCover real,
	moment timestamp,
	primary key (moment)
);

