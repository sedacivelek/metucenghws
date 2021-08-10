create table if not exists Users (
	user_id varchar,
	user_name varchar,
	review_count int,
	yelping_since timestamp,
	useful int,
	funny int,
	cool int,
	fans int,
	average_stars float8,
	primary key (user_id)
);


create table if not exists Business (
	business_id varchar,
	business_name varchar,
	address varchar,
	state varchar,
	is_open bool,
	stars float8,
	primary key (business_id)
);


create table if not exists Review (
	review_id varchar,
	user_id varchar,
	business_id varchar,
	stars float8,
	date timestamp,
	useful int,
	funny int,
	cool int,
	primary key(review_id),
	foreign key(user_id) references users(user_id),
	foreign key(business_id) references business(business_id)
);

create table if not exists Friend (
	user_id1 varchar,
	user_id2 varchar,
	primary key(user_id1,user_id2),
	foreign key(user_id1) references users(user_id),
	foreign key(user_id2) references users(user_id)
);

create table if not exists Tip (
	tip_id serial,
	business_id varchar,
	user_id varchar,
	date timestamp,
	compliment_count int,
	tip_text varchar,
	primary key(tip_id),
	foreign key(business_id) references Business(business_id),
	foreign key(user_id) references Users(user_id)
);

copy Users(user_id,user_name,review_count,yelping_since,useful,funny,cool,fans,average_stars)
from 'C:\Users\Public\dataset\yelp_academic_dataset_user.csv'
delimiter ','
csv header;

copy Business(business_id,business_name,address,state,is_open,stars)
from 'C:\Users\Public\dataset\yelp_academic_dataset_business.csv'
delimiter ','
csv header;

copy Review (review_id,user_id,business_id,stars,date,useful,funny,cool)
from 'C:\Users\Public\dataset\yelp_academic_dataset_reviewNoText.csv'
delimiter ','
csv header;

copy Friend (user_id1,user_id2)
from 'C:\Users\Public\dataset\yelp_academic_dataset_friend.csv'
delimiter ','
csv header;

copy Tip (tip_text,date,compliment_count,business_id,user_id)
from 'C:\Users\Public\dataset\yelp_academic_dataset_tip.csv'
delimiter ','
csv header;