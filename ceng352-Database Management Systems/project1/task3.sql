/*Task 3*/

/* Trigger 1 */

create function userIncrementReview() returns trigger as $IncrementReviewCount$
begin 
	update users 
	set review_count = review_count +1
	where user_id = new.user_id;
	return new;
end;
$IncrementReviewCount$ language 'plpgsql';

create trigger IncrementReviewCount 
after insert on review
for each row
execute procedure userIncrementReview();

/* Trigger 2 */

create function deleteUserReviewTip() returns trigger as $deleteWithZeroStars$
begin
	if(new.stars=0) then 
	delete from review r where r.user_id = new.user_id;
	delete from tip t where t.user_id = new.user_id;
	return new;
	end if;
end;

$deleteWithZeroStars$ language 'plpgsql';

create trigger deleteWithZeroStars
after insert on review
for each row 
execute procedure deleteUserReviewTip();

/* View */

create view BusinessCount as 
	select b.business_id , b.business_name , review_counts.review_count
	from (select r.business_id , count(*) as review_count
	from review r , business b2 
	where r.business_id = b2.business_id 
	group by r.business_id ) as review_counts, business b
	where b.business_id =review_counts.business_id;
	