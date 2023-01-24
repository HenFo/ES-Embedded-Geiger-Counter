library(tidyverse)
library(lubridate)


bl <- c(7.305760898413239, 52.135617861212616)
tr <- c(7.352510523583692, 52.157406812472374)
num_samples <- 7
num_days <- 5
num_timesteps <- num_days * 24

lat <- runif(num_samples, min = bl[2], max = tr[2])
lng <- runif(num_samples, min = bl[1], max = tr[1])
random_walk <- abs(cumsum(rnorm(num_timesteps))) + runif(1, 5, 30)

data_points <- tibble(
  Timestamp = seq.POSIXt(from=dmy_hm("01-01-2023 00:00"), by = "hour", length.out = num_timesteps),
  locations = replicate(
    length(Timestamp),
    tibble(
      lat = lat,
      lng = lng
    ),
    simplify = F
  ),
) %>%
  unnest(locations) %>%
  group_by(lat, lng) %>%
  mutate(
    count = abs(round(random_walk + rnorm(length(random_walk), sd = 3))),
  ) %>%
  nest() %>%
  ungroup() %>%
  mutate(stationID = factor(row_number())) %>%
  unnest(data)

data_points

ggplot(data_points, aes(x=Timestamp, y=count, color=stationID)) +
  geom_line()

save(data_points, file="data/samples.RData")


data_points %>%
  group_by(stationID) %>%
  nest() %>%
  ungroup() %>%
  mutate(selected = c(T, rep(F, n()-1))) %>%
  unnest(data) %>%
  ggplot(aes(x = Timestamp, y = count, color = stationID, alpha=selected, linewidth = selected)) +
  scale_alpha_manual(values = c(0.2, 1), guide = "none") +
  scale_linewidth_discrete(range = c(0.8,2)) +
  geom_line()
  # scale_linewidth_discrete(range = c(1,2))

