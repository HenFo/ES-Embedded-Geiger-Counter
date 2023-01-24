library(interp)
library(magrittr)
library(dbplyr)


interpolate <- function(locations) {
  data.inter <-
    with(locations,
         interp(
           lng,
           lat,
           count,
           method = "linear",
           nx = 100,
           ny = 100
         ))
  data.inter.tib <-
    with(data.inter, tibble(expand.grid(x = x, y = y), z = c(z)))
  data.inter.tib %<>%
    rename(count = z) %>%
    filter(!is.na(count))

  data.inter.tib
}

rgb2gray <- function(raster) {
  gray <- raster[[1]] * 0.299 + raster[[2]] * 0.587 + raster[[3]] * 0.114
  gray <- terra::rast(list(gray, gray, gray))
  gray
}


