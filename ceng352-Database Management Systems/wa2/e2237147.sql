--
-- PostgreSQL database dump
--

-- Dumped from database version 13.2
-- Dumped by pg_dump version 13.2

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: business; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.business (
    business_id character varying NOT NULL,
    business_name character varying,
    address character varying,
    state character varying,
    is_open boolean,
    stars double precision
);


ALTER TABLE public.business OWNER TO postgres;

--
-- Name: friend; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.friend (
    user_id1 character varying NOT NULL,
    user_id2 character varying NOT NULL
);


ALTER TABLE public.friend OWNER TO postgres;

--
-- Name: review; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.review (
    review_id character varying NOT NULL,
    user_id character varying,
    business_id character varying,
    stars double precision,
    date timestamp without time zone,
    useful integer,
    funny integer,
    cool integer
);


ALTER TABLE public.review OWNER TO postgres;

--
-- Name: tip; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.tip (
    tip_id integer NOT NULL,
    business_id character varying,
    user_id character varying,
    date timestamp without time zone,
    compliment_count integer,
    tip_text character varying
);


ALTER TABLE public.tip OWNER TO postgres;

--
-- Name: tip_tip_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.tip_tip_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.tip_tip_id_seq OWNER TO postgres;

--
-- Name: tip_tip_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.tip_tip_id_seq OWNED BY public.tip.tip_id;


--
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
    user_id character varying NOT NULL,
    user_name character varying,
    review_count integer,
    yelping_since timestamp without time zone,
    useful integer,
    funny integer,
    cool integer,
    fans integer,
    average_stars double precision
);


ALTER TABLE public.users OWNER TO postgres;

--
-- Name: tip tip_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.tip ALTER COLUMN tip_id SET DEFAULT nextval('public.tip_tip_id_seq'::regclass);


--
-- Name: business business_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.business
    ADD CONSTRAINT business_pkey PRIMARY KEY (business_id);


--
-- Name: friend friend_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.friend
    ADD CONSTRAINT friend_pkey PRIMARY KEY (user_id1, user_id2);


--
-- Name: review review_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.review
    ADD CONSTRAINT review_pkey PRIMARY KEY (review_id);


--
-- Name: tip tip_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.tip
    ADD CONSTRAINT tip_pkey PRIMARY KEY (tip_id);


--
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (user_id);


--
-- Name: b_bidx; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX b_bidx ON public.business USING btree (business_id);


--
-- Name: state_star_idx; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX state_star_idx ON public.business USING btree (state, stars);

ALTER TABLE public.business CLUSTER ON state_star_idx;


--
-- Name: t_bidx; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX t_bidx ON public.tip USING hash (business_id);


--
-- Name: friend friend_user_id1_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.friend
    ADD CONSTRAINT friend_user_id1_fkey FOREIGN KEY (user_id1) REFERENCES public.users(user_id);


--
-- Name: friend friend_user_id2_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.friend
    ADD CONSTRAINT friend_user_id2_fkey FOREIGN KEY (user_id2) REFERENCES public.users(user_id);


--
-- Name: review review_business_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.review
    ADD CONSTRAINT review_business_id_fkey FOREIGN KEY (business_id) REFERENCES public.business(business_id);


--
-- Name: review review_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.review
    ADD CONSTRAINT review_user_id_fkey FOREIGN KEY (user_id) REFERENCES public.users(user_id);


--
-- Name: tip tip_business_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.tip
    ADD CONSTRAINT tip_business_id_fkey FOREIGN KEY (business_id) REFERENCES public.business(business_id);


--
-- Name: tip tip_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.tip
    ADD CONSTRAINT tip_user_id_fkey FOREIGN KEY (user_id) REFERENCES public.users(user_id);


--
-- PostgreSQL database dump complete
--

