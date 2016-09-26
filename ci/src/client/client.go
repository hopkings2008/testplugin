package tests

import (
	"bytes"
	"crypto/md5"
	"encoding/base64"
	"encoding/json"
	"io"
	"io/ioutil"
	"net/http"
	"strings"
	"time"

	"gopkg.in/check.v1"
)

type UrlInfo struct {
	Path string `json:"url_path"`
}

type TestClient struct {
	*http.Client
	Root string
}

func (tc *TestClient) Upload(r io.Reader, name string, c *check.C) (UrlInfo, error) {
	var urlInfo UrlInfo
	h := md5.New()
	tee := io.TeeReader(r, h)
	content, err := ioutil.ReadAll(tee)
	if err != nil {
		c.Errorf("failed to read content, err: %v", err)
		return urlInfo, err
	}

	md5sum := h.Sum(nil)
	md5str := base64.StdEncoding.EncodeToString(md5sum)
	now := time.Now().UTC()
	date := now.Format("Mon, 02 Jan 2006 15:04:05 GMT")
	contentReader := bytes.NewReader(content)
	url := appendPath(tc.Root, name)
	req, err := http.NewRequest("PUT", url, contentReader)
	if err != nil {
		c.Errorf("Failed to create req to url(%s), err: %v", url, err)
		return urlInfo, err
	}
	//get cookie here.
	req.Header.Add("Cookie", "xxx")
	req.Header.Add("Content-MD5", md5str)
	req.Header.Add("Content-Type", "application/octet-stream")
	req.Header.Add("Date", date)
	c.Logf("req: %v", req)
	resp, err := tc.Do(req)
	if err != nil {
		c.Errorf("failed to send req to %s, err: %v", url, err)
		return urlInfo, err
	}
	defer resp.Body.Close()
	if resp.StatusCode != http.StatusOK {
		errstr, _ := ioutil.ReadAll(resp.Body)
		c.Errorf("got resp for req to %s, code: %d, err: %s", url, resp.StatusCode, errstr)
		return urlInfo, fmt.Errorf("got error response.")
	}
	urlb, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		c.Errorf("failed to read url body for %s, err: %v", url, err)
		return urlInfo, err
	}
	if err = json.Unmarshal(urlb, &urlInfo); err != nil {
		c.Errorf("got invalid url body(%s), err: %v", string(urlb), err)
		return urlInfo, err
	}
	c.Logf("got url body %s for url %s", urlInfo.Path, url)
	return urlInfo, nil
}

func (tc *TestClient) Download(path string, c *check.C) (io.ReadCloser, error) {
	url := appendPath(tc.Root, path)
	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		c.Errorf("failed to new req for %s, err: %v", url, err)
		return nil, err
	}
	//get cookie here.
	req.Header.Add("Cookie", "xxx")
	req.Header.Add("Date", date)
	c.Logf("down req: %v", req)
	resp, err := tc.Do(req)
	if err != nil {
		c.Errorf("failed to send req to %s, err: %v", url, err)
		return nil, err
	}
	if resp.StatusCode != http.StatusOK {
		defer resp.Body.Close()
		errb, _ := ioutil.ReadAll(resp.Body)
		c.Errorf("got err for req %s, err: %s", url, string(errb))
		return nil, fmt.Errorf("got err for req %s, err: %s", url, string(errb))
	}
	return resp.Body, nil
}

func appendPath(root, path string) string {
	if root == "" {
		return path
	}
	if path == "" {
		return root
	}
	if strings.HasSuffix(root, "/") {
		return root + path
	}
	return fmt.Sprintf("%s/%s", root, path)
}
