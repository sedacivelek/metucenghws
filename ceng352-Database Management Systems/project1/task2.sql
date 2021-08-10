/* Task2 */

/* Question 1 */ 
select distinct u.user_id, u.user_name, (u.review_count-u.fans) as difference
from users u,review r, business b
where u.review_count > u.fans and u.user_id = r.user_id  and b.business_id =r.business_id  and b.stars > 3.5
order by (u.review_count-u.fans) desc, u.user_id desc;

/* Question 2 */
select distinct u.user_name , b.business_name , t.date , t.compliment_count 
from users u, business b ,tip t 
where u.user_id =t.user_id and b.business_id = t.business_id and b.is_open = true and b.state = 'TX' and t.compliment_count >2
order by t.compliment_count desc, t.date desc;

/* Question 3 */

select u.user_name , friendcounts.friends
from ( select f.user_id1 , count(f.user_id2) as friends
		from friend f
		group by f.user_id1
		order by count(f.user_id2) desc
		limit 20)as friendcounts, users u
where friendcounts.user_id1 = u.user_id 
order by friendcounts.friends desc, u.user_name desc;

/* Question 4 */

select u2.user_name , u2.average_stars , u2.yelping_since
from users u2, (
				select distinct u.user_id
				from users u, review r, business b 
				where u.user_id = r.user_id and r.business_id = b.business_id and r.stars < b.stars 
				) as userTable
where userTable.user_id = u2.user_id
order by u2.average_stars desc , u2.yelping_since desc;


/* Question 5 */

select good.business_name , good.state, good.stars
from (
select b.business_id , b.business_name, b.state, b.stars ,rank() over ( order by count(*) desc) as rn
		from business b, tip t
		where b.is_open =true and b.business_id = t.business_id and t.tip_text like '%good%' and 
						date_part('year',t.date) = 2020
		group by b.business_id ) as good
where good.rn=1
order by good.stars desc , good.business_name desc;


/* Question 6 */

select distinct u.user_name , u.yelping_since, u.average_stars 
from friend f,users u
where f.user_id1 =u.user_id and u.average_stars < all (select u2.average_stars
														from users u2, friend f2 
														where f2.user_id2=u2.user_id and f2.user_id1=u.user_id)
order by u.average_stars desc,u.yelping_since desc ;

/* Question 7 */

select b.state ,avg(b.stars) as AverageOfState
from business b
group by b.state 
order by AverageOfState desc 
limit 10;

/* Question 8 */

with goodtips_cte (years,tipcount)
as (select date_part('year',t2.date)as years, count(*) as tipcount
			from tip t2 
			where t2.compliment_count > 0 
			group by date_part('year',t2.date)
			),
alltips_cte(years,tipcount)
as (select date_part('year',t2.date)as years, count(*) as tipcount ,avg(t2.compliment_count) as average
			from tip t2 
			group by date_part('year',t2.date)
			)
select allt.years, allt.average
from goodtips_cte gt, alltips_cte allt
where gt.years = allt.years and cast(gt.tipcount as float)/cast(allt.tipcount as float )  > 0.01
order by allt.years asc;
select date_part('year',t.date) ,avg(t.compliment_count)
from tip t,(select date_part('year',t2.date)as foryears, count(*) as tipcount
			from tip t2 
			where t2.compliment_count > 0 
			group by date_part('year',t2.date)
			) as goodtips
where date_part('year',t.date) = goodtips.foryears
group by date_part('year',t.date), goodtips.tipcount 
having (goodtips.tipcount/count(*)) > 0.01 ;

/* Question 9 */

select u.user_name 
from users u
where not exists ( 	select r.user_id 
						from  review r ,business b
						where r.business_id =b.business_id  and b.stars <= 3.5 and u.user_id =r.user_id 
						)
order by u.user_name asc;

/* Question 10 */ 

select b.business_name , reviews.yearpart, reviews.averagestars
from business b, (select b2.business_name ,b2.business_id, date_part('year',r.date) as yearpart , avg(r.stars) as averagestars
					from review r, business b2 
					where r.business_id =b2.business_id 
					group by yearpart,b2.business_id, b2.business_name) as reviews
where b.business_id = reviews.business_id and reviews.averagestars > 3.0 and b.business_id in (select r2.business_id
																								from review r2
																								group by r2.business_id 
																								having count(*)>1000)
order by reviews.yearpart, b.business_name;



/* Question 11 */

select u.user_name, counts.useful ,counts.cool , (counts.useful-counts.cool) as difference
from users u, (	select r.user_id as userid ,sum(r.useful) as useful, sum(r.cool) as cool
				from review r 
				group by r.user_id) as counts
where u.user_id =counts.userid and counts.useful > counts.cool
order by difference desc ,u.user_name desc;

/* Question 12*/

select distinct least(f.user_id1,f.user_id2) as user_id1,
				greatest(f.user_id1,f.user_id2) as user_id2,
				r.business_id ,r.stars 
from friend f, review r,review r2
where f.user_id1 =r.user_id and f.user_id2 = r2.user_id and r.business_id =r2.business_id and r.stars = r2.stars 
order by r.business_id desc, r.stars desc;

/* Question 13 */

select b.stars,b.state,count(*)
from business b
where b.is_open=true
group by cube((b.stars),(b.state));

/* Question 14 */

select u.user_id ,u.review_count ,u.fans , toplist.rn
from (select u.user_id ,rank () over (partition by u.fans order by u.review_count desc) as rn
		from users u
		where u.fans >= 50 and u.fans <=60)as toplist , users u
where toplist.rn<4 and toplist.user_id = u.user_id ;



