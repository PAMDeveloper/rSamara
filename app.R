#
# This is a Shiny web application. You can run the application by clicking
# the 'Run App' button above.
#
# Find out more about building applications with Shiny here:
#
#    http://shiny.rstudio.com/
#

library(shiny)
library(recomeristem)
library(ggplot2)

meteo = recomeristem::getMeteo_from_files("sample")
params = recomeristem::getParameters_from_files("sample")
result = recomeristem::rcpp_run_from_dataframe(params, meteo)[[1]]

# Define UI for application that draws a histogram
ui <- pageWithSidebar(
  headerPanel('Ecomeristem results'),
  sidebarPanel(
    tabsetPanel(
      tabPanel('Parameters',
               dataTableOutput("mytable3")),
      tabPanel('Meteo',
               dataTableOutput("mytable2")),
      tabPanel(
        'Results',
        fluidPage(
          checkboxGroupInput('show_vars',
                           'Columns in result to show:',
                           names(result),
                           selected = names(result)),
          actionLink("selectall","Select All")
        )
      )
    )
  ),
  mainPanel(
    dataTableOutput("mytable1")
  )
)

# Define server logic required to draw a histogram
server <- function(input, output, session) {

  observe({
    if(input$selectall == 0) return(NULL)
    else if (input$selectall%%2 == 0)
    {
      updateCheckboxGroupInput(session,'show_vars',
                               'Columns in result to show:',
                               names(result))
    }
    else
    {
      updateCheckboxGroupInput(session,'show_vars',
                               'Columns in result to show:',
                               names(result),
                               selected = names(result))
    }
  })

  # a large table, reative to input$show_vars
  # customize the length drop-down menu; display 5 rows per page by default
  output$mytable1 = renderDataTable({
    result[, input$show_vars, drop = FALSE]
  })

  # sorted columns are colored now because CSS are attached to them
  output$mytable2 = renderDataTable({
    meteo
  }, options = list(orderClasses = TRUE))

  # customize the length drop-down menu; display 5 rows per page by default
  output$mytable3 = renderDataTable({
    params
  }, options = list(lengthMenu = c(5, 30, 50), pageLength = 25))

}

# Run the application
shinyApp(ui = ui, server = server)

