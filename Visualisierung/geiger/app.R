library(shiny)
library(tidyverse)
library(tidyterra)
library(maptiles)
library(lubridate)
library(metR)
library(waiter)


source("helper.R")
load("data/samples.RData")

bl <- c(7.305760898413239, 52.135617861212616)
tr <- c(7.352510523583692, 52.157406812472374)
bbox <- terra::ext(c(bl[1],tr[1],bl[2],tr[2]))
steinfurt <- get_tiles(bbox, provider = "OpenStreetMap", cachedir = "data/maptiles/", crop = TRUE, verbose = TRUE)
steinfurt <- rgb2gray(steinfurt)

ui <- fluidPage(
  autoWaiter(
    html = spin_pulsar(),
    color = transparent(0.5),
    fadeout = TRUE
  ),
  titlePanel("Embedded Systems"),
  sidebarLayout(sidebarPanel(
    dateInput(
      "date",
      "Datum",
      format = "dd-mm-yyyy",
      min = floor_date(min(data_points$Timestamp), "day"),
      max = floor_date(max(data_points$Timestamp), "day")
    ),
    radioButtons(
      "agg",
      "Aggregationsfunktion über den Tag",
      choices = c("Mittelwert", "Median"),
      selected = "Mittelwert"
    )
  ),
  mainPanel(
    plotOutput("map", click = "station_click", dblclick = "clear_stations", height="500px"),
    plotOutput("timeseries")
  ))
)

server <- function(input, output) {
  # Create interpolation map ----
  current_plot_data <- NULL
  map <- reactive({
    req(input$date)

    agg <- switch (input$agg,
      "Mittelwert" = mean,
      "Median" = median
    )
    current_plot_data <<- data_points %>%
      filter(Timestamp == input$date) %>%
      group_by(stationID, lat, lng) %>%
      summarise(
        count = agg(count)
      )

    inter_data <- interpolate(current_plot_data)

    map <- ggplot(inter_data, aes(x=x, y=y)) +
      geom_spatraster_rgb(data=steinfurt) +
      geom_tile(aes(fill=count), alpha=0.8) +
      scale_fill_distiller(palette ="Spectral") +

      geom_contour(aes(z=count), color="white") +
      geom_text_contour(aes(z=count), skip = 0, color="snow4", size=3, label.placer = label_placer_fraction()) +

      geom_point(data=current_plot_data, mapping = aes(x=lng, y=lat, shape=count), shape="+", size=5) +
      ggrepel::geom_text_repel(data=current_plot_data, mapping = aes(x=lng, y=lat, label=round(count)), fontface="bold", size=5) +

      xlab("") + ylab("") + labs(fill="Count") +
      theme(panel.ontop = T,
            panel.background = element_rect(fill = NA),
            panel.grid.major = element_line(color = "gray50", linewidth = 0.25, linetype = "longdash", ),
            axis.ticks = element_blank())

    map
  })

  # Create station timeseries plot ----
  selected <- reactiveVal(rep(F, length(levels(data_points$stationID))))
  ## Add stations to selection ----
  observeEvent(input$station_click, {
    ### should never happen
    if (!exists("current_plot_data")) {
      showNotification("Ein Fehler ist aufgetreten!", type = "error")
    }
    selected(
      selected() | nearPoints(
        current_plot_data,
        xvar = "lng",
        yvar = "lat",
        threshold = 10,
        input$station_click,
        allRows = T
      )$selected_
    )
  })
  ## Clear selection ----
  observeEvent(input$clear_stations, selected(rep(F, length(levels(data_points$stationID)))))
  # Display timeseries of stations ----
  output$timeseries <- renderPlot({
    p <- if (any(selected())) {
      data_points %>%
        group_by(stationID) %>%
        nest() %>%
        ungroup() %>%
        mutate(selected = selected()) %>%
        unnest(data) %>%
        ggplot(aes(x = Timestamp, y = count, color = stationID, alpha=selected, linewidth=selected)) +
          scale_alpha_manual(values = c(0.2, 1), guide = "none") +
          scale_linewidth_discrete(range = c(0.8, 1.5), guide = "none") +
          geom_line()
    } else {
      ggplot(data_points, aes(x = Timestamp, y = count, color = stationID)) +
        geom_line(linewidth=1.5)
    }

    p + theme_bw()
  })
  # Display map ----
  output$map <- renderPlot({
    map()
  })
}

shinyApp(ui = ui, server = server)
