package main

import (
	"bytes"
	"crypto/md5"
	"encoding/base64"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"time"
)

type PathInfo struct {
	Path string `json:"url_path"`
}

func main() {
	b, err := ioutil.ReadFile("/home/zouyu/hopkings/pics/exif-orientation-examples/Landscape_3.jpg")
	if err != nil {
		fmt.Printf("failed to read file, err: %v\n", err)
		return
	}
	h := md5.New()
	n, err := h.Write(b)
	if err != nil {
		fmt.Printf("failed to md5 write, err: %v\n", err)
		return
	}
	fmt.Printf("file len: %d\n", n)
	md5sum := h.Sum(nil)
	filemd5 := base64.StdEncoding.EncodeToString(md5sum)
	now := time.Now()
	date := fmt.Sprintf("%2d, %s %4d %02d:%02d:%02d GMT", now.Day(), now.Month().String(), now.Year(), now.Hour(), now.Minute(), now.Second())
	fmt.Printf("date: %s\n", date)
	fr := bytes.NewReader(b)
	url := "http://127.0.0.1:8080/v2/img/add_image/f3t.jpg"
	//url := "http://127.0.0.1:9091/oss/upload/f3t.jpg"
	client := &http.Client{}
	req, err := http.NewRequest("PUT", url, fr)
	if err != nil {
		fmt.Printf("failed to new req, err: %v\n", err)
		return
	}
	//req.Host = "127.0.0.1:8080"
	req.Header.Add("Cookie", "xxx")
	req.Header.Add("Content-MD5", filemd5)
	req.Header.Add("Content-Type", "application/octet-stream")
	req.Header.Add("Content-Length", fmt.Sprintf("%d", n))
	req.Header.Add("Date", date)
	resp, err := client.Do(req)
	if err != nil {
		fmt.Printf("failed to send req, err: %v\n", err)
		return
	}
	fmt.Printf("resp: %v\n", resp)
	defer resp.Body.Close()
	if resp.StatusCode != http.StatusOK {
		fmt.Printf("failed to send req, got status: %d\n", resp.StatusCode)
		errstr, _ := ioutil.ReadAll(resp.Body)
		fmt.Printf("%s\n", errstr)
		return
	}
	bb, err := ioutil.ReadAll(resp.Body)
	fmt.Printf("body: %s\n", string(bb))
	var pathInfo PathInfo
	err = json.Unmarshal(bb, &pathInfo)
	if err != nil {
		fmt.Printf("got invalid path.\n")
		return
	}

	fmt.Println("******************************\n\n")

	url = "http://127.0.0.1:8080/v2/img/get_image/" + pathInfo.Path
	req, err = http.NewRequest("GET", url, nil)
	if err != nil {
		fmt.Printf("failed to new req, err: %v\n", err)
		return
	}
	req.Host = "127.0.0.1:8080"
	req.Header.Add("Cookie", "xxx")
	req.Header.Add("Date", date)
	resp, err = client.Do(req)
	if err != nil {
		fmt.Printf("failed to send req, err: %v\n", err)
		return
	}
	fmt.Printf("resp: %v\n", resp)
	if resp.StatusCode != http.StatusOK {
		fmt.Printf("got resp: %d\n", resp.StatusCode)
		return
	}
	defer resp.Body.Close()
	bb, err = ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Printf("failed to read body, err: %v\n", err)
		return
	}
	h = md5.New()
	h.Write(bb)
	md5sum = h.Sum(nil)
	dmd5 := base64.StdEncoding.EncodeToString(md5sum)
	if filemd5 != dmd5 {
		fmt.Printf("got: %s\n", dmd5)
		fmt.Printf("ori: %s\n", filemd5)
		return
	}
	fmt.Printf("download success\n")
}
